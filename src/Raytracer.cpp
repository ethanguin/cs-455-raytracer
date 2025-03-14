#include "Raytracer.h"

void Raytracer::initialize() {
    viewportHeight = 1.12;
    viewportWidth = scene.camera.aspectRatio * viewportHeight;
    viewportU = Vect3<float>(viewportWidth, 0, 0);
    viewportV = Vect3<float>(0, -viewportHeight, 0);
    dU = viewportU / static_cast<float>(scene.camera.imgWidth);
    dV = viewportV / static_cast<float>(scene.camera.imgHeight);
    viewportUpLeft = scene.camera.pos 
                    - Vect3<float>(0, 0, scene.camera.focalLength)
                    - viewportU/2.0 - viewportV/2.0;
    pixel00Pos = viewportUpLeft + (dU + dV)*.5;
}

void Raytracer::traceChunk(int startY, int endY, std::vector<pixel>& pixelList) {
    for (int j = startY; j < endY; ++j) {
        for (int i = 0; i < scene.camera.imgWidth; ++i) {
            auto pixelCenter = pixel00Pos + static_cast<float>(i) * dU + static_cast<float>(j) * dV;
            auto rayDir = pixelCenter - scene.camera.pos;
            Ray r = Ray(scene.camera.pos, rayDir);
            Color pixelColor = traceRay(r, MAX_DEPTH);
            pixelList[j * scene.camera.imgWidth + i] = pixel(pixelColor.x(), pixelColor.y(), pixelColor.z());
        }
    }
}

std::vector<pixel> Raytracer::startRaytrace() {
    std::vector<pixel> pixelList(scene.camera.imgWidth * scene.camera.imgHeight);
    std::vector<std::thread> threads;
    int chunkSize = scene.camera.imgHeight / NUM_THREADS;

    for (int i = 0; i < NUM_THREADS; ++i) {
        int startY = i * chunkSize;
        int endY = (i == NUM_THREADS - 1) ? scene.camera.imgHeight : startY + chunkSize;
        threads.emplace_back(&Raytracer::traceChunk, this, startY, endY, std::ref(pixelList));
    }

    for (auto& thread : threads) {
        thread.join();
    }
    return pixelList;
}

Color Raytracer::traceRay(const Ray &r, int depth) {
    float closestT = std::numeric_limits<float>::max();
    Object_3D* closestObj = nullptr;
    for (const auto& object : scene.objects) {
        auto currT = object->isHit(r);
        if (currT != -1 && currT < closestT) {
            closestT = currT;
            closestObj = object;
        }
    }
    if (closestObj == nullptr) {
        return toColor(scene.backgroundColor);
    }
    auto DirLight = scene.lights[0];
    // send out shadow ray
    Color baseColor;
    Normal N = closestObj->getNormal(r.at(closestT)).normal();
    bool inShadow = shadowRay(Ray(r.at(closestT), DirLight->dir), closestObj);
    if (inShadow) {
        // TOFIX add real color, this is just for debugging
        // return Color(255, 0, 0);
        baseColor = closestObj->mat.ambientColor;
    } else {
        baseColor = closestObj->mat.getLighting(N, DirLight->color, DirLight->dir, r.direction());
    }
    // return the normal at the current intersection point
    Color matColor = baseColor;
    // send out reflection ray and add it to the base color
    if (depth > 0) {
        float specularLevel = closestObj->mat.reflection;
        auto reflectionDir = (r.direction() - 2 * dot(r.direction(), N) * N).normal();
        Ray reflectionRay = Ray(r.at(closestT) + reflectionDir * 0.01, reflectionDir);
        Color reflectionColor = traceRay(reflectionRay, depth - 1);
        matColor = (reflectionColor * specularLevel) + (baseColor * (1 - specularLevel));
    }
    return matColor;
}

bool Raytracer::shadowRay(const Ray &r, const Object_3D *selfObj) {
    for (const auto& object : scene.objects) {
        if (object == selfObj) {
            continue; // Skip the object itself
        }
        auto currT = object->isHit(r);
        if (currT != -1) {
            return true; // In shadow
        }
    }
    return false; // Not in shadow
}
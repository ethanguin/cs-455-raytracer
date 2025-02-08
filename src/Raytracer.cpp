#include "Raytracer.h"

void Raytracer::initialize() {
    viewportHeight = 2.0;
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
            Color pixelColor = traceRay(r);
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

Color Raytracer::traceRay(const Ray &r) {
    Vect3<float> normDir = r.direction().normal();
    float a = .5*(normDir.y() + 1.0);
    //std::cout << "normDir.y(): " << normDir.y() << ", a: " << a << std::endl;
    Vect3<float> white = Vect3<float>(255, 255, 255) * (1 - a);
    Vect3<float> blue = Vect3<float>(127, 178, 255) * a;
    Vect3<float> combined = white + blue;
    return Color(static_cast<int>(combined.x()), static_cast<int>(combined.y()), static_cast<int>(combined.z()));
}
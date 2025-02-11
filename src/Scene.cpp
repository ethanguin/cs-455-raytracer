#include "Scene.h"

void Object_3D::move(float x, float y, float z) {
    pos += Vect3<float>(x, y, z);
}

//returns the point distance along the ray where the ray hits the sphere
float Sphere::isHit(const Ray &r) const {
    auto oc = pos - r.origin();
    //if the ray is inside the sphere, just don't render it for now
    // if (oc.length() < radius) {
    //     return -1;
    // }
    //if the ray is pointing away from the sphere, don't render it
    auto tca = dot(r.direction(), oc);
    if (tca < 0) {
        return -1;
    }
    auto tch2 = radius*radius - dot(oc, oc) + tca*tca;
    //if the ray misses the sphere, don't render it
    if (tch2 < 0) {
        return -1;
    }
    auto t = tca - sqrt(tch2);
    return t;
}

Normal Sphere::getNormal(Point3 p) const {
    return ((p - pos)/radius).normal();
}

void Scene::addSphere(float posx, float posy, float posz, float radius) {
    objects.push_back(new Sphere(posx, posy, posz, radius));
}

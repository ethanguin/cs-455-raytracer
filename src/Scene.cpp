#include "Scene.h"

void Object_3D::move(float x, float y, float z) {
    pos += Vect3<float>(x, y, z);
}

bool Object_3D::isHit(const Ray &r) const {
    return false;
}

bool Sphere::isHit(const Ray &r) const {
    Vect3<float> oc = pos - r.origin();
    auto a = dot(r.direction(), r.direction());
    auto b = -2.0 * dot(r.direction(), oc);
    auto c = dot(oc, oc) - radius*radius;
    auto discriminant = b*b - 4*a*c;
    return (discriminant >= 0);
}
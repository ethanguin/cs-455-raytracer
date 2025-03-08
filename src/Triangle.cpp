#include "Triangle.h"
#include <algorithm>

float Triangle::isHit(const Ray &r) const {
    // Moller-Trumbore intersection algorithm
    const float EPSILON = 1e-8;
    Vect3<float> edge1 = v1 - v0;
    Vect3<float> edge2 = v2 - v0;
    Vect3<float> h = r.direction().cross(edge2);
    float a = dot(edge1, h);
    if (a > -EPSILON && a < EPSILON) {
        return -1; // This ray is parallel to this triangle.
    }
    float f = 1.0 / a;
    Vect3<float> s = r.origin() - v0;
    float u = f * dot(s, h);
    if (u < 0.0 || u > 1.0) {
        return -1;
    }
    Vect3<float> q = s.cross(edge1);
    float v = f * dot(r.direction(), q);
    if (v < 0.0 || u + v > 1.0) {
        return -1;
    }
    // At this stage we can compute t to find out where the intersection point is on the line.
    float t = f * dot(edge2, q);
    if (t > EPSILON) { // ray intersection
        return t;
    } else { // This means that there is a line intersection but not a ray intersection.
        return -1;
    }
}

Normal Triangle::getNormal(Point3 p) const {
    return normal;
}

void Triangle::move(float x, float y, float z) {
    // transform to 0, 0, 0
    Matrix4 translationMatrix = Matrix4::translation(-v0.x(), -v0.y(), -v0.z());
    transform(translationMatrix);
    // move to position x, y, z
    translationMatrix = Matrix4::translation(x, y, z);
    transform(translationMatrix);
    // update position
    pos = Point3(x, y, z);
}

void Triangle::transform(const Matrix4& mat) {
    v0 = mat.transform(v0);
    v1 = mat.transform(v1);
    v2 = mat.transform(v2);
    normal = calcNormal();
}

Normal Triangle::calcNormal() const {
    Vect3<float> edge1 = v1 - v0;
    Vect3<float> edge2 = v2 - v0;
    Vect3<float> normal = edge1.cross(edge2).normal();
    return Normal(normal.x(), normal.y(), normal.z());
}

void Triangle::moveAdd(float x, float y, float z) {
    pos += Point3(x, y, z);
    Matrix4 translationMatrix = Matrix4::translation(x, y, z);
    transform(translationMatrix);
}

void TriShape::addTriangle(const Point3 v0, const Point3 v1, const Point3 v2) {
    triangles.push_back(new Triangle(v0, v1, v2));
}

float TriShape::isHit(const Ray &r) const {
    float closestT = std::numeric_limits<float>::max();
    for (const auto& triangle : triangles) {
        float t = triangle->isHit(r);
        if (t != -1 && t < closestT) {
            closestT = t;
        }
    }
    return (closestT == std::numeric_limits<float>::max()) ? -1 : closestT;
}

Normal TriShape::getNormal(Point3 p) const {
    for (const auto& triangle : triangles) {
        // Check if the point p lies on the triangle
        if (triangle->isHit(Ray(p, triangle->getNormal(p))) != -1) {
            return triangle->getNormal(p);
        }
    }
    // If no triangle is found, return a default normal
    return Normal(0, 0, 0);
}

void TriShape::move(float x, float y, float z) {
    Matrix4 translationMatrix = Matrix4::translation(x, y, z);
    for (const auto& triangle : triangles) {
        triangle->transform(translationMatrix);
    }
    pos = Point3(x, y, z);
}
void TriShape::rotate(float anglex, float angley, float anglez) {
    Matrix4 rotationMatrix = Matrix4::rotation(anglex, angley, anglez);
    for (const auto& triangle : triangles) {
        triangle->transform(rotationMatrix);
    }

}

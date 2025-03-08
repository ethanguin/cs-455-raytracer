#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Scene.h"
#include "Matrix4.h"


class Triangle : public Object_3D {
public:
    Point3 v0, v1, v2; // Vertices of the triangle
    Normal normal;

    Triangle() {}

    Triangle(const Point3 v0, const Point3 v1, const Point3 v2) 
        : v0(v0), v1(v1), v2(v2) {
            normal = calcNormal();
        }
    Triangle(const Point3 position, const float size) {
        v0 = position + Vect3<float>(0, 0, 0);
        v1 = position + Vect3<float>(size, 0, 0);
        v2 = position + Vect3<float>(size/2, size, 0);
    }

    float isHit(const Ray &r) const override;
    Normal getNormal(Point3 p) const override;
    void move(float x, float y, float z) override;
    void moveAdd(float x, float y, float z);
    void transform(const Matrix4& mat) override;
    // void rotate(float x, float y, float z);
    Normal calcNormal() const;
};

class TriShape : public Object_3D {
public:
    std::vector<Triangle*> triangles;

    TriShape() {}

    TriShape(const std::vector<Triangle*> triangles) 
        : triangles(triangles) {}

    void addTriangle(const Point3 v0, const Point3 v1, const Point3 v2);
    float isHit(const Ray &r) const override;
    Normal getNormal(Point3 p) const override;
    void move(float x, float y, float z) override;
    void rotate(float x, float y, float z);
};

#endif
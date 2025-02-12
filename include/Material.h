#ifndef MATERIAL_H
#define MATERIAL_H
#include "Vect3.h"
#include <cmath>

class Material {
public:
    bool metal = false;
    float mAmbient = 0.1;
    Color ambientColor = Color(0, 0, 0);
    Color specColor = Color(1, 1, 1);
    Color baseColor = Color(50, 50, 50);
    float kgls = 16;
    float kd = 0.7;
    float ks = 0.2;
    float ka = 0.1;
    Material() {}
    Material(Color baseColor) : baseColor(baseColor) {}
    Color getLighting(Normal &N, const Color &LightColor, const Point3 &LightDir, const Point3 &ViewVector);
    void setBaseColor(Color new_color);
    void setSpecColor(Color new_color);
    Color clip(Color nVect, float lower, float upper);
};

#endif
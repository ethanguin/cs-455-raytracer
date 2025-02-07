#ifndef RAY_H
#define RAY_H
#include "Vect3.h"

class Ray {
    private:
        Point3 orig;
        Vect3<float> dir;
    public:
        Ray() {}
        Ray(const Point3 &origin_input, const Vect3<float> &direction_input) {
            orig = origin_input;
            dir = direction_input;
        }
        const Point3 origin() const { return orig; }
        const Vect3<float> direction() const { return dir; }
        Point3 at(float t) const {
            return orig + t * dir;
        }
};

#endif
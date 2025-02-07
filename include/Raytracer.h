#ifndef RAYTRACER_H
#define RAYTRACER_H
#include <list>
#include <random>
#include "pixel.h"
#include "Scene.h"
#include "Vect3.h"

class Raytracer {
    public:
        Scene scene;
        Raytracer() {}
        Raytracer(Scene new_scene) {
            scene = new_scene;
        }
        std::list<pixel> startRaytrace();
        
};

#endif
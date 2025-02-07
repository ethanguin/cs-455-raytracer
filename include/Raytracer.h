#ifndef RAYTRACER_H
#define RAYTRACER_H
#include <list>
#include <random>
#include "pixel.h"
#include "Scene.h"
#include "Ray.h"
#include "Vect3.h"

class Raytracer {
    private:
        float viewportHeight;
        float viewportWidth;
        Vect3<float> viewportU;
        Vect3<float> viewportV;
        Vect3<float> dU; //size of each pixel in x
        Vect3<float> dV; //size of each pixel in y
        Vect3<float> viewportUpLeft;
        Vect3<float> pixel00Pos;
    public:
        Scene scene;
        Raytracer() {
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
        Raytracer(Scene new_scene) {
            Raytracer();
            scene = new_scene;
        }
        std::list<pixel> startRaytrace();
        Color traceRay(const Ray &r);
        
};

#endif
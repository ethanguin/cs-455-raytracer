#ifndef RAYTRACER_H
#define RAYTRACER_H
#include <vector>
#include <random>
#include <thread>
#include "pixel.h"
#include "Scene.h"
#include "Ray.h"
#include "loadbar.h"
#include "Vect3.h"

class Raytracer {
    private:
        const int NUM_THREADS = 2;
        const int MAX_DEPTH = 3;
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
        Raytracer (int width, int height) {
            scene.camera.imgWidth = width;
            scene.camera.imgHeight = height;
            initialize();
        }
        Raytracer() {
            initialize();
        }
        Raytracer(Scene new_scene) {
            scene = new_scene;
            initialize();
        }
        void initialize();
        std::vector<pixel> startRaytrace();
        void traceChunk(int startY, int endY, std::vector<pixel>& pixelList);
        Color traceRay(const Ray &r, int depth);
        bool shadowRay(const Ray &r, const Object_3D *selfObj);
};

#endif
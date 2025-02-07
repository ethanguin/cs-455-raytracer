#include "Raytracer.h"

std::list<pixel> Raytracer::startRaytrace() {
    std::list<pixel> pixelList;
    //put the pixels into a list
    for (int i = 0; i < scene.camera.width; i++){
        for (int j = 0; j < scene.camera.height; j++){
            pixelList.push_back(pixel(rand()%255, rand()%255, rand()%255));
        }
    }
    return pixelList;
}
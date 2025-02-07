#include "Raytracer.h"

std::list<pixel> Raytracer::startRaytrace() {
    std::list<pixel> pixelList;
    //put the pixels into a list
    for (int j = 0; j < scene.camera.height; j++){
        std::clog << "\rScanlines remaining: " << (scene.camera.height - j) << ' ' << std::flush;
        for (int i = 0; i < scene.camera.width; i++){
            pixelList.push_back(pixel(rand()%255, rand()%255, rand()%255));
        }
    }
    std::clog << "\rDone.                 \n";
    return pixelList;
}
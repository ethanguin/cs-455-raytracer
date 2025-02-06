#include <iostream>
#include "Raytracer.h"

class Raytracer {
    public:
        Scene scene;
        Raytracer() {}
        Raytracer(Scene new_scene) {
            scene = new_scene;
        }
        std::list<pixel> startRaytrace() {
            //define a background color to replace when raytracing & testing image writing
            pixel defaultPixel;
            defaultPixel.R = 255;
            defaultPixel.G = 0;
            defaultPixel.B = 255;

            std::list<pixel> pixelList;
            //put the pixels into a list
            for (int i = 0; i < scene.camera.width; i++){
                for (int j = 0; j < scene.camera.height; j++){
                    pixelList.push_back(defaultPixel);
                    std::cout << pixelList << std::endl;
                }
            }
        }
        
};
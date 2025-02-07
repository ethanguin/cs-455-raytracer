#include <iostream>
#include <fstream>
#include <list>
#include <string>
#include "pixel.h"
#include "Raytracer.h"

void exportImage(const std::list<pixel> &pixel_values, int width, int height) {
    std::ofstream fileOut("output.ppm");
    if (!fileOut) {
        std::cerr << "Error: Could not open output.ppm for writing." << std::endl;
        return;
    }
    fileOut << "P3\n" << width << ' ' << height << "\n255\n";
    for (const auto &pix : pixel_values) {
        fileOut << pix << '\n';
    }
    fileOut.close();
}

int main() {
    //pass properties to RayTracer
    
    Raytracer raytracer = Raytracer();
    //get the image, currently just purple pixels throughout
    std::list<pixel> image = raytracer.startRaytrace();
    exportImage(image, raytracer.scene.camera.width, raytracer.scene.camera.height);
    return 1;
}
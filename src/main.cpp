#include <iostream>
#include <fstream>
#include <filesystem>
#include <list>
#include <string>
#include "pixel.h"
#include "Raytracer.h"

bool is_file_exist(std::string fileName) {
    std::ifstream infile(fileName);
    return infile.good();
}

void exportImage(const std::list<pixel> &pixel_values, int width, int height) {
    std::string filename = "Raytrace_output0.ppm";
    while (is_file_exist(filename)) {
        int num = 1;
        filename = "Raytrace_output" + std::to_string(num) + ".ppm";
    }
    std::ofstream fileOut(filename);
    if (!fileOut) {
        std::cerr << "Error: Could not open output.ppm for writing." << std::endl;
        return;
    }
    fileOut << "P3\n" << width << ' ' << height << "\n255\n";
    for (const auto &pix : pixel_values) {
        fileOut << pix << '\n';
    }
    fileOut.close();
    return;
}

int main() {
    //pass properties to RayTracer
    
    Raytracer raytracer = Raytracer();
    //get the image, currently just purple pixels throughout
    std::list<pixel> image = raytracer.startRaytrace();
    exportImage(image, raytracer.scene.camera.width, raytracer.scene.camera.height);
    return 1;
}
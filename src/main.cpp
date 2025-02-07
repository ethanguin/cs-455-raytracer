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

void exportImage(std::string filename, const std::list<pixel> &pixel_values, int width, int height) {
    int num = 0;
    while (is_file_exist(filename)) {
        num++;
        filename = filename.substr(0, filename.find_last_of(".")); //remove extension
        filename = filename + std::to_string(num) + ".ppm";
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

int main(int argc, char* argv[]) {
    //pass properties to RayTracer
    std::string filename = "output.ppm";
    if (argc > 1) {
        filename = argv[1];
    }
    Raytracer raytracer = Raytracer();
    //get the image, currently just purple pixels throughout
    std::list<pixel> image = raytracer.startRaytrace();
    exportImage(filename, image, raytracer.scene.camera.width, raytracer.scene.camera.height);
    return 1;
}
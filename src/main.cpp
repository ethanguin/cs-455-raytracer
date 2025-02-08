#include <iostream>
#include <fstream>
#include <filesystem>
#include <thread>
#include <vector>
#include <chrono>
#include <string>
#include "pixel.h"
#include "Raytracer.h"

bool is_file_exist(std::string fileName) {
    std::ifstream infile(fileName);
    return infile.good();
}

void exportImage(std::string fileName, const std::vector<pixel> &pixel_values, int width, int height) {
    int num = 0;
    std::string fileBase = fileName.substr(0, fileName.find_last_of(".")); //remove extension
    while (is_file_exist(fileName)) {
        num++;
        fileName = fileBase + std::to_string(num) + ".ppm";
    }
    std::ofstream fileOut(fileName);
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
    std::string fileName = "output.ppm";
    if (argc > 1) {
        fileName = argv[1];
    }
    fileName = "../output/" + fileName;
    Raytracer raytracer = Raytracer(100, 100);
    //get the image, currently just a blue gradient
    std::vector<pixel> image = raytracer.startRaytrace();
    exportImage(fileName, image, raytracer.scene.camera.imgWidth, raytracer.scene.camera.imgHeight);
    return 1;
}
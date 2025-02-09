#include <iostream>
#include <fstream>
#include <filesystem>
#include <thread>
#include <vector>
#include <chrono>
#include <string>
#include "loadbar.h"
#include "pixel.h"
#include "Scene.h"
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
    std::cout << "Exporting image to " << fileName << "..." << std::endl;
    int count = 0;
    for (const auto &pix : pixel_values) {
        fileOut << pix << '\n';
        count++;
        if (count % width*10 == 0) {
            showLoadingBar(count, width * height);
        }
    }
    fileOut.close();
    return;
}

int main(int argc, char* argv[]) {
    auto start = std::chrono::high_resolution_clock::now(); // Start timer
    //pass properties to RayTracer
    std::string fileName = "output.ppm";
    if (argc > 1) {
        fileName = argv[1];
    }
    fileName = "../output/" + fileName;
    Scene scene = Scene();
    scene.addSphere(0, 0, -1, .5);
    scene.camera.imgWidth = 1920;
    scene.camera.imgHeight = 1080;
    Raytracer raytracer = Raytracer(scene);
    std::vector<pixel> image = raytracer.startRaytrace();
    auto endRay = std::chrono::high_resolution_clock::now(); // End timer
    std::chrono::duration<double> durationRay = endRay - start;
    std::cout << "Raytracing Done at: " << durationRay.count() << " seconds" << std::endl;
    exportImage(fileName, image, raytracer.scene.camera.imgWidth, raytracer.scene.camera.imgHeight);
    auto end = std::chrono::high_resolution_clock::now(); // End timer
    std::chrono::duration<double> duration = end - start;
    std::cout << std::endl << "Time taken: " << duration.count() << " seconds" << std::endl;
    return 1;
}
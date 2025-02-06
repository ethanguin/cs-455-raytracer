#include <iostream>
#include <fstream>
#include <list>
#include <string>
#include "pixel.h"
#include "Raytracer.h"

void exportImage(std::list<pixel> &pixel_values, int width, int height) {
    std::ofstream fileOut("output.txt");
    std::streambuf *coutbuf = std::cout.rdbuf();
    std::cout.rdbuf(fileOut.rdbuf());
    std::cout << "P3\n" << width << ' ' << height << "\n255\n";
    for (pixel pix : pixel_values) {
        std::cout << pix.R << ' ' << pix.G << ' ' << pix.B << '\n';
    } 
    std::cout.rdbuf(coutbuf);
}

int main() {
    //pass properties to RayTracer
    Raytracer raytracer = Raytracer();
    //get the image, currently just purple pixels throughout
    std::list<pixel> image = raytracer.startRaytrace();
    //exportImage(image, raytracer.scene.camera.width, raytracer.scene.camera.height);
    return 1;
}
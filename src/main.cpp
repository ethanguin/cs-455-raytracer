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
    // while (is_file_exist(fileName)) {
    //     num++;
    //     fileName = fileBase + "_" + std::to_string(num) + ".ppm";
    // }
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
        // if (count % width*10 == 0) {
        //     showLoadingBar(count, width * height);
        // }
    }
    fileOut.close();
    return;
}

int main(int argc, char* argv[]) {
    auto start = std::chrono::high_resolution_clock::now(); // Start timer
    //pass properties to RayTracer
    std::string fileName = "EthanBishop_Raytracing-Test1.ppm";
    if (argc > 1) {
        fileName = argv[1];
    }
    fileName = "../output/" + fileName;
    Scene scene1 = Scene();
    scene1.setBackgroundColor(Color(.2, .2, .2));
    //scene1.addSphere(0, 0, -1, .5);
    Object_3D *sphere = new Sphere(0, 0, -1, .4);
    sphere->mat.setBaseColor(Color(255, 0, 255));
    sphere->mat.setSpecColor(Color(255, 255, 255));
    scene1.addObject(sphere);

    scene1.addLight(new Light_Directional(0, 1, 0));
    Camera camera = Camera(1000, 1000);
    camera.move(0, 0, 1);
    camera.lookAt(Point3(0, 0, 0));
    scene1.setCamera(camera);
    Raytracer raytracer = Raytracer(scene1);
    std::vector<pixel> image = raytracer.startRaytrace();
    auto endRay1 = std::chrono::high_resolution_clock::now(); // End timer
    std::chrono::duration<double> durationRay = endRay1 - start;
    std::cout << "Raytracing Done at: " << durationRay.count() << " seconds" << std::endl;
    exportImage(fileName, image, raytracer.scene.camera.imgWidth, raytracer.scene.camera.imgHeight);

    //Second Scene
    Scene scene2 = Scene();
    scene2.setCamera(camera);
    scene2.setBackgroundColor(Color(.2, .2, .2));
    scene2.addLight(new Light_Directional(1, 1, 1));
    
    //---------WHITE SPHERE----------------//
     Color ambColor = Color(.1, .1, .1);
     Sphere *sphere1 = new Sphere(.45, 0, -.15, .15);
     sphere1->mat.ambientColor = ambColor;
     sphere1->mat.kd = .8;
     sphere1->mat.ks = .1;
     sphere1->mat.ka = .3;
     sphere1->mat.setBaseColor(Color(255, 255, 255));
     sphere1->mat.setSpecColor(Color(255, 255, 255));
     sphere1->mat.kgls = 4.0;
     scene2.addObject(sphere1);
    //----------------------------------//
    
    //---------RED SPHERE----------------//
    Sphere *sphere2 = new Sphere(0, 0, -.1, .2);
    sphere2->mat.ambientColor = ambColor;
    sphere2->mat.kd = .6;
    sphere2->mat.ks = .3;
    sphere2->mat.ka = .1;
    sphere2->mat.setBaseColor(Color(255, 0, 0));
    sphere2->mat.setSpecColor(Color(255, 255, 255));
    sphere2->mat.kgls = 32.0;
    scene2.addObject(sphere2);
    //----------------------------------//

    //---------GREEN SPHERE----------------//
    Sphere *sphere3 = new Sphere(-.6, 0, 0, .3);
    sphere3->mat.ambientColor = ambColor;
    sphere3->mat.kd = .7;
    sphere3->mat.ks = .2;
    sphere3->mat.ka = .1;
    sphere3->mat.setBaseColor(Color(0, 255, 0));
    sphere3->mat.setSpecColor(Color(255/2, 255, 255/2));
    sphere3->mat.kgls = 64.0;
    scene2.addObject(sphere3);
    //----------------------------------//

    //---------BLUE SPHERE----------------//
    Sphere *sphere4 = new Sphere(0, -10000.5, 0, 10000.0);
    sphere4->mat.ambientColor = ambColor;
    sphere4->mat.kd = .8;
    sphere4->mat.ks = .1;
    sphere4->mat.ka = .3;
    sphere4->mat.setBaseColor(Color(0, 0, 255));
    sphere4->mat.setSpecColor(Color(255, 255, 255));
    sphere4->mat.kgls = 16.0;
    scene2.addObject(sphere4);
    //----------------------------------//

    std::string file2 = "EthanBishop_Raytracing-Test2.ppm";
    fileName = "../output/" + file2;
    Raytracer raytracer2 = Raytracer(scene2);
    std::vector<pixel> image2 = raytracer2.startRaytrace();
    auto endRay2 = std::chrono::high_resolution_clock::now(); // End timer
    std::chrono::duration<double> durationRay2 = endRay2 - start;
    std::cout << "Raytracing Done at: " << durationRay2.count() << " seconds" << std::endl;
    exportImage(fileName, image2, raytracer.scene.camera.imgWidth, raytracer.scene.camera.imgHeight);


    // My own test case
    Scene scene3 = Scene();
    scene3.setCamera(camera);
    scene3.camera.move(0, 0, 1.5);
    scene3.setBackgroundColor(Color(0, .66, 1));
    scene3.addLight(new Light_Directional(-.5, .5, 1));
    ambColor = Color(.2, .2, 0);
    
    //SUN
    Sphere *bigSphere = new Sphere(450, 450, -1000, 300);
    bigSphere->mat.ambientColor = Color(5, 5, 0);
    bigSphere->mat.setBaseColor(Color(255, 255, 0));
    bigSphere->mat.kd = .5;
    bigSphere->mat.ks = .4;
    bigSphere->mat.ka = .1;
    scene3.addObject(bigSphere);

    // Sphere *tinySphere = new Sphere(-1, 0, 0, .1);
    // tinySphere->mat.ambientColor = ambColor;
    // scene3.addObject(tinySphere);

    //---------Grass--------------------//
    Point3 pos = Point3(0, -2.2, -.5);
    float rad = 2;
    Sphere *grass = new Sphere(pos.x(), pos.y(), pos.z(), rad);
    grass->mat.ambientColor = ambColor;
    grass->mat.kd = .8;
    grass->mat.ks = .2;
    grass->mat.ka = 0;
    grass->mat.setBaseColor(Color(0, 255, 0));
    grass->mat.setSpecColor(Color(255, 255, 255));
    grass->mat.kgls = 2.0;
    scene3.addObject(grass);
    //----------------------------------//

    //---------Blue Sphere--------------------//
    float rad2 = .2;
    Sphere *blue = new Sphere(pos.x(), pos.y()+rad+rad2, pos.z(), rad2);
    blue->mat.ambientColor = ambColor;
    blue->mat.kd = .5;
    blue->mat.ks = .4;
    blue->mat.ka = .1;
    blue->mat.setBaseColor(Color(0, 100, 255));
    blue->mat.setSpecColor(Color(255, 255, 255));
    blue->mat.kgls = 18.0;
    scene3.addObject(blue);
    //----------------------------------//
    //---------Red Sphere--------------------//
    Sphere *red = new Sphere(-rad2*1.45, pos.y()+rad+rad2/1.8, pos.z(), rad2/2);
    red->mat.ambientColor = ambColor;
    red->mat.kd = .5;
    red->mat.ks = .4;
    red->mat.ka = .1;
    red->mat.setBaseColor(Color(255, 0, 0));
    red->mat.setSpecColor(Color(255, 255, 255));
    red->mat.kgls = 50.0;
    scene3.addObject(red);
    //----------------------------------//
    //---------Black Sphere--------------------//
    Sphere *black = new Sphere(rad2*1.45, pos.y()+rad+rad2/1.8, pos.z(), rad2/2);
    black->mat.ambientColor = ambColor;
    black->mat.kd = .1;
    black->mat.ks = .8;
    black->mat.ka = .1;
    black->mat.setBaseColor(Color(10, 10, 10));
    black->mat.setSpecColor(Color(255, 255, 255));
    black->mat.kgls = 15.0;
    scene3.addObject(black);
    //----------------------------------//

    file2 = "EthanBishop_Raytracing-Test3.ppm";
    fileName = "../output/" + file2;
    Raytracer raytracer3 = Raytracer(scene3);
    std::vector<pixel> image3 = raytracer3.startRaytrace();
    exportImage(fileName, image3, raytracer.scene.camera.imgWidth, raytracer.scene.camera.imgHeight);

    return 1;
}
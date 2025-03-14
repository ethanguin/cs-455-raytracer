#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <iostream>
#include <fstream>
#include <filesystem>
#include <thread>
#include <vector>
#include <chrono>
#include <string>
#include <iomanip>
#include "loadbar.h"
#include "stb_image_write.h"
#include "stb_image.h"
#include "pixel.h"
#include "Triangle.h"
#include "Scene.h"
#include "Raytracer.h"

const float pi = 3.14159265359;
void createTestFrame(int i, int maxFrames);
void renderTestAnimation() {
    int frameNum = 96;

    for (int i = 0; i < frameNum; i++) {
        createTestFrame(i, frameNum);
    }
    // bypass multithread
    return;
    std::vector<std::thread> threads;
    // Launch a thread for each frame
    
    for (int i = 0; i < frameNum; i++) {
        // create a thread for every frame of my test animation
        threads.emplace_back(createTestFrame, i, frameNum);
    }

    // Join all threads
    for (auto& thread : threads) {
        thread.join();
    }
}
std::string fillWithZeros(int number, int width) {
    std::stringstream ss;
    ss << std::setw(width) << std::setfill('0') << number;
    return ss.str();
}

bool is_file_exist(std::string fileName) {
    std::ifstream infile(fileName);
    return infile.good();
}
void exportPNG(std::string filename, const std::vector<pixel> &pixel_values, int width, int height, int fileNum) {
    filename += "." + fillWithZeros(fileNum, 4) + ".png";
    std::cout << "Exporting image to " << filename << "..." << std::endl;
    std::vector<unsigned char> imageData(width * height * 3);
    for (size_t i = 0; i < pixel_values.size(); ++i) {
        imageData[i * 3 + 0] = static_cast<unsigned char>(pixel_values[i].R());
        imageData[i * 3 + 1] = static_cast<unsigned char>(pixel_values[i].G());
        imageData[i * 3 + 2] = static_cast<unsigned char>(pixel_values[i].B());
    }
    stbi_write_png(filename.c_str(), width, height, 3, imageData.data(), width * 3);
}
void exportPPM(std::string fileName, const std::vector<pixel> &pixel_values, int width, int height, int fileNum) {
    int num = 0;
    std::string fileBase = fileName.substr(0, fileName.find_last_of(".")); //remove extension
    // while (is_file_exist(fileName)) {
    //     num++;
    //     fileName = fileBase + "_" + std::to_string(num) + ".ppm";
    // }
    std::ofstream fileOut(fileName.append("." + fillWithZeros(fileNum, 4) + ".ppm"));
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

std::vector<Object_3D*> createStar(Point3 pos = Point3(0, 0, 0)) {
    std::vector<Object_3D*> starShape = std::vector<Object_3D*>();
    float thickness = .1;
    float edgeLen = .2;
    float innerLen = .1;

    // center vertices
    Point3 centerTop = Point3(pos.x(), pos.y(), pos.z() + thickness/2);
    Point3 centerBot = Point3(pos.x(), pos.y(), pos.z() - thickness/2);
    // outer vertices
    Point3 top = Point3(pos.x(), pos.y() + edgeLen, pos.z());
    Point3 topR = Point3(pos.x() + std::cos(pi/2 - 2*pi/5)*edgeLen, pos.y() + std::sin(pi/2 - 2*pi/5)*edgeLen, pos.z() + 0);
    Point3 topL = Point3(pos.x() + std::cos(pi/2 + 2*pi/5)*edgeLen, pos.y() + std::sin(pi/2 + 2*pi/5)*edgeLen, pos.z() + 0);
    Point3 botL = Point3(pos.x() + std::cos(3*pi/2-pi/5)*edgeLen, pos.y() + std::sin(3*pi/2-pi/5)*edgeLen, pos.z() + 0);
    Point3 botR = Point3(pos.x() + std::cos(3*pi/2+pi/5)*edgeLen, pos.y() + std::sin(3*pi/2+pi/5)*edgeLen, pos.z() + 0);
    // inner vertices
    Point3 topRinner = Point3(pos.x() + std::cos(pi/2 - pi/5)*innerLen, pos.y() + std::sin(pi/2 - pi/5)*innerLen, pos.z() + 0);
    Point3 topLinner = Point3(pos.x() + std::cos(pi/2 + pi/5)*innerLen, pos.y() + std::sin(pi/2 + pi/5)*innerLen, pos.z() + 0);
    Point3 botLinner = Point3(pos.x() + std::cos(3*pi/2 - 2*pi/5)*innerLen, pos.y() + std::sin(3*pi/2 - 2*pi/5)*innerLen, pos.z() + 0);
    Point3 botRinner = Point3(pos.x() + std::cos(3*pi/2 + 2*pi/5)*innerLen, pos.y() + std::sin(3*pi/2 + 2*pi/5)*innerLen, pos.z() + 0);
    Point3 botInner = Point3(pos.x() + 0, pos.y() - innerLen, pos.z() + 0);

    // top going counter-clockwise
    starShape.push_back(new Triangle(centerTop, topRinner, top));
    starShape.push_back(new Triangle(centerTop, topR, topRinner));
    starShape.push_back(new Triangle(centerTop, botRinner, topR));
    starShape.push_back(new Triangle(centerTop, botR, botRinner));
    starShape.push_back(new Triangle(centerTop, botInner, botR));
    starShape.push_back(new Triangle(centerTop, botL, botInner));
    starShape.push_back(new Triangle(centerTop, botLinner, botL));
    starShape.push_back(new Triangle(centerTop, topL, botLinner));
    starShape.push_back(new Triangle(centerTop, topLinner, topL));
    starShape.push_back(new Triangle(centerTop, top, topLinner));

    // bottom going clockwise (counter-clockwise when viewed from the bottom)
    starShape.push_back(new Triangle(centerBot, top, topRinner));
    starShape.push_back(new Triangle(centerBot, topRinner, topR));
    starShape.push_back(new Triangle(centerBot, topR, botRinner));
    starShape.push_back(new Triangle(centerBot, botRinner, botR));
    starShape.push_back(new Triangle(centerBot, botR, botInner));
    starShape.push_back(new Triangle(centerBot, botInner, botL));
    starShape.push_back(new Triangle(centerBot, botL, botLinner));
    starShape.push_back(new Triangle(centerBot, botLinner, topL));
    starShape.push_back(new Triangle(centerBot, topL, topLinner));
    starShape.push_back(new Triangle(centerBot, topLinner, top));

    
    return starShape;
}

void createTestFrame(int i, int maxFrames) {
    std::string fileName = "EthanBishop_Raytracing_Pt2";
    fileName = "../output/" + fileName;

    float pi = 3.14159265359;
    float circleRad = .65;

    Scene scene1 = Scene();
    Color ambLight = Color(.6, .6, .6);
    scene1.setBackgroundColor(Color(.2, .2, .2));

    // star made of Tris
    std::vector<Object_3D*> star = createStar(Point3(0, std::cos(i * 2 * pi / maxFrames) * .22, 0));
    for (auto tri : star) {
        tri->transform(Matrix4::rotation(0, 2*pi/(maxFrames/2)*i, 0));
    }
    for (auto tri : star) {
        tri->mat.setBaseColor(Color(255, 241, 87));
        tri->mat.setSpecColor(tri->mat.baseColor);
        tri->mat.ks = .45;
        tri->mat.kd = .45;
        tri->mat.ka = .1;
        tri->mat.kgls = 20;
        tri->mat.ambientColor = ambLight;
    }
    scene1.addObjectList(star);

    // basic purple sphere
    // Object_3D *sphere = new Sphere(0, 0, 0, .4);
    // sphere->mat.setBaseColor(Color(255, 0, 255));
    // sphere->mat.setSpecColor(Color(255, 255, 255));
    // sphere->mat.ambientColor = ambLight;
    // scene1.addObject(sphere);
    // block light, shadow test
    Object_3D *sphereBlock = new Sphere(0, std::cos(i * 2 * pi / maxFrames) * circleRad*.95, -std::sin(i * 2 * pi / maxFrames) * circleRad*.95, .05);
    sphereBlock->mat.setBaseColor(Color(50, 50, 50));
    sphereBlock->mat.setSpecColor(Color(255, 255, 255));
    sphereBlock->mat.ambientColor = ambLight;
    scene1.addObject(sphereBlock);
    // reflect light test1
    Object_3D *sphereReflect = new Sphere(std::cos(i * 2 * pi / maxFrames) * circleRad, 0, std::sin(i * 2 * pi / maxFrames) * circleRad, .1);
    sphereReflect->mat.setBaseColor(Color(0, 180, 250));
    sphereReflect->mat.setSpecColor(Color(255, 255, 255));
    sphereReflect->mat.ambientColor = ambLight;
    scene1.addObject(sphereReflect);

    scene1.addLight(new Light_Directional(.5, 1, .1));
    Camera camera = Camera(2000, 2000);
    camera.move(0, 0, 2);
    camera.lookAt(Point3(0, 0, 0));
    scene1.setCamera(camera);
    Raytracer raytracer = Raytracer(scene1);
    raytracer.setThreadNum(16);
    std::vector<pixel> image = raytracer.startRaytrace();
    exportPNG(fileName, image, raytracer.scene.camera.imgWidth, raytracer.scene.camera.imgHeight, i);
}

void testScene1() {
    std::string fileName = "EthanBishop_Raytracing_Pt2_Test1";
    fileName = "../output/" + fileName;

    float pi = 3.14159265359;
    float circleRad = .65;

    Scene scene1 = Scene();
    Color ambLight = Color(0, 0, 0);
    scene1.setBackgroundColor(Color(.2, .2, .2));

    //reflective sphere
        //     # reflective sphere
        // Sphere
        // Center 0.0 0.3 -1.0
        // Radius 0.25
        // Kd 0.0
        // Ks 0.1
        // Ka 0.1
        // Od 0.75 0.75 0.75
        // Os 1.0 1.0 1.0
        // Kgls 10.0
        // Refl .9
    Object_3D *sphereReflect = new Sphere(0, .3, -1, .25);
    sphereReflect->mat.setBaseColor(Color(191, 191, 191));
    sphereReflect->mat.setSpecColor(Color(255, 255, 255));
    sphereReflect->mat.ambientColor = ambLight;
    sphereReflect->mat.ks = .4;
    sphereReflect->mat.kd = 0;
    sphereReflect->mat.ka = .1;
    sphereReflect->mat.kgls = 10;
    sphereReflect->mat.reflection = .9;
    scene1.addObject(sphereReflect);

    //triangle 1 - yellow
    // # yellow triangle
    // Triangle
    // 0.0, -0.7, -0.5
    // 0.0, -0.7, -1.5
    // -1.0, 0.4, -1.0
    // Kd 0.9
    // Ks 1.0
    // Ka 0.1
    // Od 1.0 1.0 0.0
    // Os 1.0 1.0 1.0
    // Kgls 4.0
    // Refl 0.0
    Object_3D *triangle1 = new Triangle(Point3(0, -.7, -.5), Point3(0, -.7, -1.5), Point3(-1, .4, -1));
    triangle1->mat.setBaseColor(Color(255, 255, 0));
    triangle1->mat.setSpecColor(Color(255, 255, 255));
    triangle1->mat.ambientColor = ambLight;
    triangle1->mat.ks = 1;
    triangle1->mat.kd = .9;
    triangle1->mat.ka = .1;
    triangle1->mat.kgls = 4;
    triangle1->mat.reflection = 0;
    scene1.addObject(triangle1);

    //triangle 2 - blue
    // # blue triangle
    // Triangle
    // 0.0 -0.7 -0.5
    // 1.0, 0.4, -1.0
    // 0.0, -0.7, -1.5
    // Kd 0.9
    // Ks 1.0
    // Ka 0.1
    // Od 0.0 0.0 1.0
    // Os 1.0 1.0 1.0
    // Kgls 4.0
    // Refl 0.0
    Object_3D *triangle2 = new Triangle(Point3(0, -.7, -.5), Point3(1, .4, -1), Point3(0, -.7, -1.5));
    triangle2->mat.setBaseColor(Color(0, 0, 255));
    triangle2->mat.setSpecColor(Color(255, 255, 255));
    triangle2->mat.ambientColor = ambLight;
    triangle2->mat.ks = 1;
    triangle2->mat.kd = .9;
    triangle2->mat.ka = .1;
    triangle2->mat.kgls = 4;
    triangle2->mat.reflection = 0;
    scene1.addObject(triangle2);

    scene1.addLight(new Light_Directional(0, 1, 0));
    Camera camera = Camera(2000, 2000);
    camera.move(0, 0, 1);
    camera.lookAt(Point3(0, 0, 0));
    scene1.setCamera(camera);
    Raytracer raytracer = Raytracer(scene1);
    raytracer.setThreadNum(32);
    std::vector<pixel> image = raytracer.startRaytrace();
    exportPNG(fileName, image, raytracer.scene.camera.imgWidth, raytracer.scene.camera.imgHeight, 0);
}

void testScene2() {
    std::string fileName = "EthanBishop_Raytracing_Pt2_Test2";
    fileName = "../output/" + fileName;

    float pi = 3.14159265359;
    float circleRad = .65;

    Scene scene1 = Scene();
    Color ambLight = Color(0, 0, 0);
    scene1.setBackgroundColor(Color(.2, .2, .2));

    // white sphere
    // Sphere
    // Center 0.5 0.0 -0.15
    // Radius 0.05
    // Kd 0.8
    // Ks 0.1
    // Ka 0.3
    // Od 1.0, 1.0, 1.0
    // Os 1.0, 1.0, 1.0
    // Kgls 4.0
    // Refl 0.0
    Object_3D *sphereWhite = new Sphere(.5, 0, -.15, .05);
    sphereWhite->mat.setBaseColor(Color(255, 255, 255));
    sphereWhite->mat.setSpecColor(Color(255, 255, 255));
    sphereWhite->mat.ambientColor = ambLight;
    sphereWhite->mat.ks = .1;
    sphereWhite->mat.kd = .8;
    sphereWhite->mat.ka = .3;
    sphereWhite->mat.kgls = 4;
    sphereWhite->mat.reflection = 0;
    scene1.addObject(sphereWhite);

    // red sphere
    // Sphere
    // Center 0.3 0.0 -0.1
    // Radius 0.08
    // Kd 0.8
    // Ks 0.8
    // Ka 0.1
    // Od 1.0 0.0 0.0
    // Os 0.5 1.0 0.5
    // Kgls 32.0
    // Refl 0.0
    Object_3D *sphereRed = new Sphere(.3, 0, -.1, .08);
    sphereRed->mat.setBaseColor(Color(255, 0, 0));
    sphereRed->mat.setSpecColor(Color(127, 255, 127));
    sphereRed->mat.ambientColor = ambLight;
    sphereRed->mat.ks = .8;
    sphereRed->mat.kd = .8;
    sphereRed->mat.ka = .1;
    sphereRed->mat.kgls = 32;
    sphereRed->mat.reflection = 0;
    scene1.addObject(sphereRed);

    // green sphere
    // Sphere
    //   Center -0.6 0.0 0.0
    //   Radius .3
    //   Kd 0.7
    //   Ks 0.5
    //   Ka 0.1
    //   Od 0.0 1.0 0.0
    //   Os 0.5 1.0 0.5
    //   Kgls 64.0
    //   Refl 0.0
    Object_3D *sphereGreen = new Sphere(-.6, 0, 0, .3);
    sphereGreen->mat.setBaseColor(Color(0, 255, 0));
    sphereGreen->mat.setSpecColor(Color(127, 255, 127));
    sphereGreen->mat.ambientColor = ambLight;
    sphereGreen->mat.ks = .5;
    sphereGreen->mat.kd = .7;
    sphereGreen->mat.ka = .1;
    sphereGreen->mat.kgls = 64;
    sphereGreen->mat.reflection = 0;
    scene1.addObject(sphereGreen);

    //     # reflective sphere
    // Sphere
    // Center 0.1 -0.55 0.25
    // Radius 0.3
    // Kd 0.0
    // Ks 0.1
    // Ka 0.1
    // Od 0.75 0.75 0.75
    // Os 1.0 1.0 1.0
    // Kgls 10.0
    // Refl 0.9
    Object_3D *sphereReflect = new Sphere(.1, -.55, .25, .3);
    sphereReflect->mat.setBaseColor(Color(191, 191, 191));
    sphereReflect->mat.setSpecColor(Color(255, 255, 255));
    sphereReflect->mat.ambientColor = ambLight;
    sphereReflect->mat.ks = .1;
    sphereReflect->mat.kd = 0;
    sphereReflect->mat.ka = .1;
    sphereReflect->mat.kgls = 10;
    sphereReflect->mat.reflection = .9;
    scene1.addObject(sphereReflect);

    //     # blue triangle
    // Triangle
    // 0.3 -0.3 -0.4
    // 0.0 0.3 -0.1
    // -0.3 -0.3 0.2
    // Kd 0.9
    // Ks 0.9
    // Ka 0.1
    // Od 0.0 0.0 1.0
    // Os 1.0 1.0 1.0
    // Kgls 32.0
    // Refl 0.0
    Object_3D *triangleBlue = new Triangle(Point3(.3, -.3, -.4), Point3(0, .3, -.1), Point3(-.3, -.3, .2));
    triangleBlue->mat.setBaseColor(Color(0, 0, 255));
    triangleBlue->mat.setSpecColor(Color(255, 255, 255));
    triangleBlue->mat.ambientColor = ambLight;
    triangleBlue->mat.ks = .9;
    triangleBlue->mat.kd = .9;
    triangleBlue->mat.ka = .1;
    triangleBlue->mat.kgls = 32;
    triangleBlue->mat.reflection = 0;
    scene1.addObject(triangleBlue);

    //     # yellow triangle
    // Triangle
    // -0.2 0.1 0.1
    // -0.2 -0.5 0.2
    // -0.2 0.1 -0.3
    // Kd 0.9
    // Ks 0.5
    // Ka 0.1
    // Od 1.0 1.0 0.0
    // Os 1.0 1.0 1.0
    // Kgls 4.0
    // Refl 0.0
    Object_3D *triangleYellow = new Triangle(Point3(-.2, .1, .1), Point3(-.2, -.5, .2), Point3(-.2, .1, -.3));
    triangleYellow->mat.setBaseColor(Color(255, 255, 0));
    triangleYellow->mat.setSpecColor(Color(255, 255, 255));
    triangleYellow->mat.ambientColor = ambLight;
    triangleYellow->mat.ks = .5;
    triangleYellow->mat.kd = .9;
    triangleYellow->mat.ka = .1;
    triangleYellow->mat.kgls = 4;
    triangleYellow->mat.reflection = 0;
    scene1.addObject(triangleYellow);

    scene1.addLight(new Light_Directional(1, 0, 0));
    Camera camera = Camera(2000, 2000);
    camera.move(0, 0, 1);
    camera.lookAt(Point3(0, 0, 0));
    scene1.setCamera(camera);
    Raytracer raytracer = Raytracer(scene1);
    raytracer.setThreadNum(32);
    std::vector<pixel> image = raytracer.startRaytrace();
    exportPNG(fileName, image, raytracer.scene.camera.imgWidth, raytracer.scene.camera.imgHeight, 0);
}

int main(int argc, char* argv[]) {
    auto start = std::chrono::high_resolution_clock::now(); // Start timer
    std::string fileName = "Triangle";
    if (argc > 1) {
        fileName = argv[1];
    }
    fileName = "../output/" + fileName;
    
    
    // // testing triangle objects
    // Scene scene1 = Scene();
    // Color ambLight = Color(.6, .6, .6);
    // scene1.setBackgroundColor(Color(.2, .2, .2));
    

    // scene1.addLight(new Light_Directional(.1, 1, .15));
    // Camera camera = Camera(2000, 2000);
    // camera.move(0, 0, 2);
    // camera.lookAt(Point3(0, 0, 0));
    // scene1.setCamera(camera);
    // Raytracer raytracer = Raytracer(scene1);
    // std::vector<pixel> image = raytracer.startRaytrace();
    // exportPNG(fileName, image, raytracer.scene.camera.imgWidth, raytracer.scene.camera.imgHeight, 0);

    // render an animation multithreaded
    //renderTestAnimation();

    testScene1();
    testScene2();
    

    auto endRay1 = std::chrono::high_resolution_clock::now(); // End timer
    std::chrono::duration<double> durationRay = endRay1 - start;
    std::cout << "Raytracing Done at: " << durationRay.count() << " seconds" << std::endl;

    return 0;
}
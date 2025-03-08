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

    // Create a vector to hold the threads
    // for (int i = 0; i < frameNum; i++) {
    //     createTestFrame(i, frameNum);
    // }
    // // bypass multithread
    //return;
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
        tri->mat.setSpecColor(Color(255, 255, 255));
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
    std::vector<pixel> image = raytracer.startRaytrace();
    exportPNG(fileName, image, raytracer.scene.camera.imgWidth, raytracer.scene.camera.imgHeight, i);
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
    renderTestAnimation();
    

    auto endRay1 = std::chrono::high_resolution_clock::now(); // End timer
    std::chrono::duration<double> durationRay = endRay1 - start;
    std::cout << "Raytracing Done at: " << durationRay.count() << " seconds" << std::endl;

    return 0;
}
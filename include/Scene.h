#ifndef SCENE_H
#define SCENE_H
#include "Vect3.h"
#include "Ray.h"
#include "Matrix4.h"
#include "Material.h"
#include <vector>

class Object_3D {
    public:
        Vect3<float> pos = Vect3<float>(0, 0, 0);
        Vect3<float> rot = Vect3<float>(90, 0, 0);
        Vect3<float> dir = Vect3<float>(0, 0, -1);
        Material mat = Material();
        Object_3D() {}
        Object_3D(Vect3<float> pos_input, Vect3<float> rot_input) {
            pos = pos_input;
            rot = rot_input;
        }
        Object_3D(float posx,float posy,float posz,float rotx,float roty,float rotz) {
            pos = Vect3<float>(posx, posy, posz);
            rot = Vect3<float>(rotx, roty, rotz);
        }
        virtual void move(float x, float y, float z);
        virtual float isHit(const Ray &r) const {
            return -1;
        }
        virtual Normal getNormal(Point3 ) const {
            return Normal(0, 0, 0);
        }
        virtual void transform (const Matrix4& mat) {
            pos = mat.transform(pos);
            rot = mat.transform(rot);
        }
        ~Object_3D() = default;
};

class Sphere : public Object_3D {
    public:
        float radius;
        Sphere() {
            radius = 1.0;
        }
        Sphere(float posx, float posy, float posz, float radius_input) {
            pos = Vect3<float>(posx, posy, posz);
            radius = radius_input;
        }
        float isHit(const Ray &r) const override;
        Normal getNormal(Point3 p) const override;
};

class Camera : public Object_3D {
    public:
        int imgWidth;
        int imgHeight;
        float aspectRatio;
        float fov;
        float focalLength;
        Camera() {
            imgWidth = 1920;
            imgHeight = 1080;
            aspectRatio = (double)imgWidth / (double)imgHeight;
            fov = 90.0;
            focalLength = 1.0;
            rot = Vect3<float>(0, 0, -1);
        }
        Camera(int width, int height) {
            imgWidth = width;
            imgHeight = height;
            aspectRatio = (double)imgWidth / (double)imgHeight;
            fov = 90.0;
            focalLength = 1.0;
            rot = Vect3<float>(0, 0, -1);
        }
        void lookAt(Point3 target) {
            dir = (target - pos).normal();
        }
};

class Light : public Object_3D {
    public:
        Color color;
        int intensity;
        Vect3<float> dir = Vect3<float>(0, 0, 0);
        Light() {
            color = Color(1, 1, 1);
            intensity = 1;
        }
        void setColor(Color new_color) {
            color = new_color;
        }
};
class Light_Directional : public Light {
    public:
        Light_Directional() {
            dir = Vect3<float>(-1, -1, -.1).normal();
        }
        Light_Directional(float dirx, float diry, float dirz) {
            dir = Vect3<float>(dirx, diry, dirz);
            dir = dir.normal();
        }
};

class Scene {
    friend class Raytracer;
    private:
        std::vector<Object_3D*> objects;
        std::vector<Light*> lights;
        Color backgroundColor = Color(0, 0, 0);
    public:
        Camera camera;
        Scene(){}
        void addSphere(float posx, float posy, float posz, float radius);
        void addLight(Light* light) {
            lights.push_back(light);
        }
        void setCamera(Camera new_camera) {
            camera = new_camera;
        }
        void addObject(Object_3D* obj) {
            objects.push_back(obj);
        }
        void addObjectList(std::vector<Object_3D*> objs) {
            for (int i = 0; i < objs.size(); i++) {
                objects.push_back(objs[i]);
            }
        }
        void setBackgroundColor(Color new_color);
};

#endif
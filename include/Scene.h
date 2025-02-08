#ifndef SCENE_H
#define SCENE_H
#include "Vect3.h"
#include "Ray.h"
#include <list>

class Object_3D {
    public:
        Vect3<float> pos;
        Vect3<float> rot;
        Object_3D() {
            pos = Vect3<float>(0, 0, 0);
            rot = Vect3<float>(0, 0, 0);
        }
        Object_3D(Vect3<float> pos_input, Vect3<float> rot_input) {
            pos = pos_input;
            rot = rot_input;
        }
        Object_3D(float posx,float posy,float posz,float rotx,float roty,float rotz) {
            pos = Vect3<float>(posx, posy, posz);
            rot = Vect3<float>(rotx, roty, rotz);
        }
        void move(float x, float y, float z);
        virtual bool isHit(const Ray &r) const;
};

class Polygon : public Object_3D {
    std::list<Vect3<int> > verts;
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
        bool isHit(const Ray &r) const override;
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
        }
};

class Light : public Object_3D {
    public:
        Color color;
        int intensity;
        Light() {}
};
class Light_Directional : public Light {
    Vect3<float> dir;
    Light_Directional() {
        dir = Vect3<float>(-1, 0, 0);
    }
    Light_Directional(float posx, float posy, float posz, float dirx, float diry, float dirz) {
        dir = Vect3<float>(dirx, diry, dirz);
        dir = dir.normal();
        pos = Vect3<float>(posx, posy, posz);
    }
};

class Scene {
    public:
        std::list<Object_3D> objects;
        Camera camera;
        std::list<Light> lights;
        Scene(){}
        void addSphere(float posx, float posy, float posz, float radius) {
            objects.push_back(Sphere(posx, posy, posz, radius));
        }
};

#endif
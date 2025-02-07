#ifndef SCENE_H
#define SCENE_H
#include "Vect3.h"
#include <list>

class Object_3D {
    public:
        vect3<int> pos;
        vect3<int> rot;
        Object_3D() {
            pos = vect3<int>(0, 0, 0);
            rot = vect3<int>(0, 0, 0);
        }
        Object_3D(vect3<int> pos_input, vect3<int> rot_input) {
            pos = pos_input;
            rot = rot_input;
        }
        Object_3D(int posx,int posy,int posz,int rotx,int roty,int rotz) {
            pos = vect3<int>(posx, posy, posz);
            rot = vect3<int>(rotx, roty, rotz);
        }
        void move(int x, int y, int z);
};

class Polygon : public Object_3D {
    std::list<vect3<int> > verts;
};

class Camera : public Object_3D {
    public:
        int width;
        int height;
        Camera() {
            width = 1920;
            height = 1080;
        }
};

class Light : public Object_3D {
    public:
        vect3<int> color;
        int intensity;
        Light() {}
};
class Light_Directional : public Light {
    vect3<float> dir;
    Light_Directional() {
        dir = vect3<float>(-1, 0, 0);
    }
    Light_Directional(int posx, int posy, int posz, float dirx, float diry, float dirz) {
        dir = vect3<float>(dirx, diry, dirz);
        dir = dir.normal();
        pos = vect3<int>(posx, posy, posz);
    }
};

class Scene {
    public:
        std::list<Object_3D> objects;
        Camera camera;
        std::list<Light> lights;
        Scene(){}
};

#endif
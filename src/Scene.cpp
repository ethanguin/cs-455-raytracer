#include "Scene.h"

void Object_3D::move(float x, float y, float z) {
    pos += Vect3<float>(x, y, z);
}
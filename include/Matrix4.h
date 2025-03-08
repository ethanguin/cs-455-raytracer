#ifndef MATRIX4
#define MATRIX4

#include "Vect3.h"

class Matrix4 {
public:
    float m[4][4];

    Matrix4() {
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                m[i][j] = (i == j) ? 1.0f : 0.0f;
            }
        }
    }

    static Matrix4 translation(float x, float y, float z) {
        Matrix4 mat;
        mat.m[0][3] = x;
        mat.m[1][3] = y;
        mat.m[2][3] = z;
        return mat;
    }

    Vect3<float> transform(const Vect3<float>& v) const {
        float x = v.x() * m[0][0] + v.y() * m[0][1] + v.z() * m[0][2] + m[0][3];
        float y = v.x() * m[1][0] + v.y() * m[1][1] + v.z() * m[1][2] + m[1][3];
        float z = v.x() * m[2][0] + v.y() * m[2][1] + v.z() * m[2][2] + m[2][3];
        return Vect3<float>(x, y, z);
    }

    static Matrix4 rotation(float x, float y, float z) {
        Matrix4 mat;
        mat.m[0][0] = cos(y) * cos(z);
        mat.m[0][1] = cos(y) * sin(z);
        mat.m[0][2] = -sin(y);
        mat.m[1][0] = sin(x) * sin(y) * cos(z) - cos(x) * sin(z);
        mat.m[1][1] = sin(x) * sin(y) * sin(z) + cos(x) * cos(z);
        mat.m[1][2] = sin(x) * cos(y);
        mat.m[2][0] = cos(x) * sin(y) * cos(z) + sin(x) * sin(z);
        mat.m[2][1] = cos(x) * sin(y) * sin(z) - sin(x) * cos(z);
        mat.m[2][2] = cos(x) * cos(y);
        return mat;
    }
};

#endif
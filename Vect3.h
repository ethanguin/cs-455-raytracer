#ifndef VECT3_H
#define VECT3_H
#include <vector>
#include <cmath>

template <typename T>
class Vect3 {    
    private:
        T x;
        T y;
        T z;
    public:
        std::vector<T> v {x, y, z};
        Vect3() {
            x = 0;
            y = 0;
            z = 0;
        }
        Vect3(T x_input, T y_input, T z_input) {
            x = x_input;
            y = y_input;
            z = z_input;
        }
        Vect3<float> normalized() {
            float length = std::sqrt(x + y + z);
            return Vect3<float>(float(x)/length, float(y)/length, float(z)/length);
        }
};

#endif
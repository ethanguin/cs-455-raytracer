#ifndef VECT3_H
#define VECT3_H
#include <array>
#include <cmath>
#include <iostream>

template <typename T>
class vect3 {    
    public:
        std::array<T, 3> v;
        vect3() {
            v = {0, 0, 0};
        }
        vect3(T x_input, T y_input, T z_input) {
            v = {x_input, y_input, z_input};
        }

        vect3<float> normal() {
            float len = this->length();
            return vect3<float>(float(v[0])/len, float(v[1])/len, float(v[2])/len);
        }
        T x() { return v[0]; }
        T y() { return v[1]; }
        T z() { return v[2]; }
        vect3 operator-() const { return vect3(-v[0], -v[1], -v[2]); }
        T operator[](int i) const { return v[i]; }
        T& operator[](int i) { return v[i]; }

        vect3& operator+=(const vect3& e) {
            v[0] += e.v[0];
            v[1] += e.v[1];
            v[2] += e.v[2];
            return *this;
        }

        vect3& operator*=(T t) {
            v[0] *= t;
            v[1] *= t;
            v[2] *= t;
            return *this;
        }
        
        vect3& operator/=(T t) {
            return *this *= 1/t;
        }

        float length() const {
            return std::sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
        }

        vect3<T> cross(const vect3<T>& e) const {
            return vect3<T>(v[1] * e.v[2] - v[2] * e.v[1],
                            v[2] * e.v[0] - v[0] * e.v[2],
                            v[0] * e.v[1] - v[1] * e.v[0]);
        }
        template <typename U>
        friend std::ostream& operator<<(std::ostream& out, const vect3<U>& e);
};
using point3 = vect3<int>;
using color = vect3<int>;

template <typename T>
inline std::ostream& operator<<(std::ostream& out, const vect3<T>& e) {
    return out << e.v[0] << ' ' << e.v[1] << ' ' << e.v[2];
}
template <typename T>
inline vect3<T> operator+(const vect3<T>& u, const vect3<T>& e) {
    return vect3<T>(u.v[0] + e.ev[0], u.v[1] + e.v[1], u.v[2] + e.v[2]);
}
template <typename T>
inline vect3<T> operator-(const vect3<T>& u, const vect3<T>& e) {
    return vect3<T>(u.v[0] - e.v[0], u.v[1] - e.v[1], u.v[2] - e.v[2]);
}
template <typename T>
inline vect3<T> operator*(const vect3<T>& u, const vect3<T>& e) {
    return vect3<T>(u.v[0] * e.v[0], u.v[1] * e.v[1], u.v[2] * e.v[2]);
}
template <typename T>
inline vect3<T> operator*(float t, const vect3<T>& e) {
    return vect3<T>(t*e.v[0], t*e.v[1], t*e.v[2]);
}
template <typename T>
inline vect3<T> operator*(const vect3<T>& v, float t) {
    return t * v;
}
template <typename T>
inline vect3<T> operator/(const vect3<T>& v, float t) {
    if (t == 0) throw std::runtime_error("Division by zero");
    return v *= (1 / t);
}
template <typename T>
inline float dot(const vect3<T>& u, const vect3<T>& e) {
    return u.v[0] * e.v[0]
         + u.v[1] * e.v[1]
         + u.v[2] * e.v[2];
}
template <typename T>
inline vect3<T> cross(const vect3<T>& u, const vect3<T>& e) {
    return vect3<T>(u.v[1] * e.v[2] - u.v[2] * e.v[1],
                    u.v[2] * e.v[0] - u.v[0] * e.v[2],
                    u.v[0] * e.v[1] - u.v[1] * e.v[0]);
}
template <typename T>
inline vect3<T> unit_vector(const vect3<T>& e) {
    return e / e.length();
}

#endif
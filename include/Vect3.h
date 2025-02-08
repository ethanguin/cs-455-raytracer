#ifndef VECT3_H
#define VECT3_H
#include <array>
#include <cmath>
#include <iostream>

template <typename T>
class Vect3 {    
    public:
        std::array<T, 3> v;
        Vect3() {
            v = {0, 0, 0};
        }
        Vect3(T x_input, T y_input, T z_input) {
            v = {x_input, y_input, z_input};
        }

        Vect3<T> normal() const {
            float len = this->length();
            return Vect3<float>(float(v[0]) / len, float(v[1]) / len, float(v[2]) / len);
        }
        T x() { return v[0]; }
        T y() { return v[1]; }
        T z() { return v[2]; }
        Vect3 operator-() const { return Vect3(-v[0], -v[1], -v[2]); }
        T operator[](int i) const { return v[i]; }
        T& operator[](int i) { return v[i]; }

        Vect3& operator+=(const Vect3& e) {
            v[0] += e.v[0];
            v[1] += e.v[1];
            v[2] += e.v[2];
            return *this;
        }

        Vect3& operator*=(T t) {
            v[0] *= t;
            v[1] *= t;
            v[2] *= t;
            return *this;
        }
        
        Vect3& operator/=(T t) {
            return *this *= 1/t;
        }

        float length() const {
            return std::sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
        }

        Vect3<T> cross(const Vect3<T>& e) const {
            return Vect3<T>(v[1] * e.v[2] - v[2] * e.v[1],
                            v[2] * e.v[0] - v[0] * e.v[2],
                            v[0] * e.v[1] - v[1] * e.v[0]);
        }
        template <typename U>
        friend std::ostream& operator<<(std::ostream& out, const Vect3<U>& e);
};
using Point3 = Vect3<float>;
using Color = Vect3<int>;

template <typename T>
inline std::ostream& operator<<(std::ostream& out, const Vect3<T>& e) {
    return out << e.v[0] << ' ' << e.v[1] << ' ' << e.v[2];
}
template <typename T, typename U>
inline Vect3<T> operator+(const Vect3<T>& u, const Vect3<U>& e) {
    return Vect3<T>(u.v[0] + e.v[0], u.v[1] + e.v[1], u.v[2] + e.v[2]);
}
template <typename T, typename U>
inline Vect3<T> operator-(const Vect3<T>& u, const Vect3<U>& e) {
    return Vect3<T>(u.v[0] - e.v[0], u.v[1] - e.v[1], u.v[2] - e.v[2]);
}
template <typename T, typename U>
inline Vect3<T> operator*(const Vect3<T>& u, const Vect3<U>& e) {
    return Vect3<T>(u.v[0] * e.v[0], u.v[1] * e.v[1], u.v[2] * e.v[2]);
}
template <typename T, typename U>
inline Vect3<T> operator*(T t, const Vect3<U>& e) {
    return Vect3<T>(t*e.v[0], t*e.v[1], t*e.v[2]);
}
template <typename T, typename U>
inline Vect3<T> operator*(const Vect3<T>& e, U t) {
    return Vect3<T>(t*e.v[0], t*e.v[1], t*e.v[2]);
}
template <typename T, typename U>
inline Vect3<T> operator/(const Vect3<T>& v, U t) {
    if (t == 0) throw std::runtime_error("Division by zero");
    return v * (1 / t);
}
template <typename T, typename U>
inline float dot(const Vect3<T>& u, const Vect3<U>& e) {
    return u.v[0] * e.v[0]
         + u.v[1] * e.v[1]
         + u.v[2] * e.v[2];
}
template <typename T, typename U>
inline Vect3<T> cross(const Vect3<T>& u, const Vect3<U>& e) {
    return Vect3<T>(u.v[1] * e.v[2] - u.v[2] * e.v[1],
                    u.v[2] * e.v[0] - u.v[0] * e.v[2],
                    u.v[0] * e.v[1] - u.v[1] * e.v[0]);
}
template <typename T>
inline Vect3<T> unit_vector(const Vect3<T>& e) {
    return e / e.length();
}

#endif
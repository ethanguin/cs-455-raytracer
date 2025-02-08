#ifndef PIXEL_H
#define PIXEL_H
#include <string>
#include "Vect3.h"
#include <iostream>

struct pixel {
    Vect3<int> RGB;
    pixel() {}
    pixel(int R_in, int G_in, int B_in) {
      RGB[0] = R_in;
      RGB[1] = G_in;
      RGB[2] = B_in;
    }
    int R() const { return RGB[0]; }
    int G() const { return RGB[1]; }
    int B() const { return RGB[2]; }
    std::string toString () {
        return std::to_string(R()) + " " + std::to_string(G()) + " " + std::to_string(B()) + "\n";
    }
};

inline std::ostream & operator<<(std::ostream& os, const pixel& v) { 
  os << std::to_string(v.R()) << " " << std::to_string(v.G()) << " " << std::to_string(v.B()) << "\n";
  return os;
}

#endif
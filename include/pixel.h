#ifndef PIXEL_H
#define PIXEL_H
#include <string>
#include <iostream>

struct pixel {
    int R, G, B;
    pixel() {}
    pixel(int R_in, int G_in, int B_in) {
      R = R_in;
      G = G_in;
      B = B_in;
    }
    // int A; //for later if I implement exporting with png with alpha
    std::string toString () {
        return std::to_string(R) + ' ' + std::to_string(G) + ' ' + std::to_string(B) + '\n';
    }
};

inline std::ostream & operator<<(std::ostream& os, const pixel& v) { 
  os << std::to_string(v.R) << " " << std::to_string(v.G) << " " << std::to_string(v.B) << "\n";
  return os;
}

#endif
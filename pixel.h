#ifndef PIXEL_H
#define PIXEL_H
#include <string>

struct pixel {
    int R, G, B;
    // int A; //for later if I implement exporting with png with alpha
    std::string toString () {
        return std::to_string(R) + ' ' + std::to_string(G) + ' ' + std::to_string(B) + '\n';
    }
};

inline std::ostream & operator<<(std::ostream& os, const pixel& v) { 
  // print something from v to str, e.g: Str << v.getX();
  os << v.R << ' ' << v.G << ' ' << v.B << '\n';
  return os;
}

template <typename Container,
          typename = std::enable_if_t<
              std::is_same_v<typename Container::value_type, pixel>>>
inline std::ostream& operator<<(std::ostream& os, const Container& pixels) {
    os << "[";
    bool first = true;
    for (const auto& pixel : pixels) {
        if (!first) os << ", ";
        os << pixel;
        first = false;
    }
    os << "]";
    return os;
}

#endif
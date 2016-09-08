#ifndef COLOR_H
#define COLOR_H

#include <cstdint>

namespace display3r {

struct Color {
    static Color RED;
    static Color GREEN;
    static Color BLUE;
    
    uint8_t r;
    uint8_t g;
    uint8_t b;
    Color();
    Color(uint8_t r, uint8_t g, uint8_t b);
    
    Color const &operator*(float a);
};

};
#endif // COLOR_H

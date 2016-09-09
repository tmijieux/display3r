#ifndef COLOR_H
#define COLOR_H

#include <cstdint>
#include <string>

namespace display3r {

struct Color {
    static Color RED;
    static Color GREEN;
    static Color BLUE;
    static Color WHITE;
    static Color BLACK;

    uint8_t r;
    uint8_t g;
    uint8_t b;
    Color();
    Color(uint8_t r, uint8_t g, uint8_t b);

    Color operator*(float a) const;

    Color operator+(Color const &C) const;
    Color operator-(Color const &C) const;
    Color operator*(Color const &C) const;

    void Filter(Color const &o);
    void Average(Color const &o, unsigned char scale);

    static Color FromString(std::string const&);
};


Color Interpolate(Color const &A, Color const &B, Color const &C,
                  float a, float b, float c);

};
#endif // COLOR_H

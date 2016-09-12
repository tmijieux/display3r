#ifndef DISPLAY3R_COLOR_H
#define DISPLAY3R_COLOR_H

#include <cstdint>
#include <string>
#include <ostream>

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
    Color operator*(Color const &C) const;

    void Filter(Color const &o);
    void Average(Color const &o, unsigned char scale);

    static Color FromString(std::string const&);
    static Color Interpolate(
        Color const &A, Color const &B, Color const &C,
        float a, float b, float c);
};

}; // end namespace display3r

namespace std {

static inline ostream &operator<<(ostream &s, display3r::Color const &c)
{
    s << (int)c.r << " " << (int)c.g << " " << (int)c.b;
    return s;
}

};  // end namespace std

#endif // DISPLAY3R_COLOR_H

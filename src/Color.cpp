#include <sstream>
#include "display3r/Color.hpp"
#include "display3r/Util.hpp"

using display3r::Color;

Color Color::RED(255, 0, 0);
Color Color::GREEN(0, 255, 0);
Color Color::BLUE(0, 0, 255);
Color Color::BLACK(0, 0, 0);
Color Color::WHITE(255, 255, 255);

Color::Color(uint8_t r_, uint8_t g_, uint8_t b_):
    r(r_), g(g_), b(b_) {}

Color::Color():
    r(0), g(0), b(0) {}


Color Color::operator*(float a) const
{
    Color c;
    c.r = r * a / 255.;
    c.g = g * a / 255.;
    c.b = b * a / 255;
    return c;
}

Color Color::operator-(Color const &o) const
{
    Color c;
    c.r = MAX(0, r - o.r);
    c.g = MAX(0, g - o.g);
    c.b = MAX(0, b - o.b);
    return c;
}

Color Color::operator+(Color const &o) const
{
    Color c;
    c.r = MIN(255, r + o.r);
    c.g = MIN(255, g + o.g);
    c.b = MIN(255, b + o.b);
    return c;
}

void Color::Filter(Color const &o)
{
    Color c;
    r = r * o.r / 255.;
    g = g * o.g / 255.;
    b = b * o.b / 255.;
}

Color Color::operator*(Color const &o) const
{
    Color c;
    c.r = (unsigned)r * (unsigned)o.r / 255;
    c.g = (unsigned)g * (unsigned)o.g / 255;
    c.b = (unsigned)b * (unsigned)o.b / 255;
}

void Color::Average(Color const &o, unsigned char scale)
{
    r = r + (o.r - r) * scale / 255;
    g = g + (o.g - g) * scale / 255;
    b = b + (o.b - b) * scale / 255;
}

Color Color::FromString(std::string const &s)
{
    Color c;
    std::stringstream ss(s);
    ss.imbue(std::locale("C"));
    ss >> c.r >> c.g >> c.b;
    return c;
}

namespace display3r {

Color Interpolate(Color const &A, Color const &B, Color const &C,
                  float a, float b, float c)
{
    Color d;
    d.r = a*A.r+b*B.r+c*C.r;
    d.g = a*A.g+b*B.g+c*C.g;
    d.b = a*A.b+b*B.b+c*C.b;
    return d;
}




};

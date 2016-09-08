
#include "display3r/Color.hpp"

using display3r::Color;

Color Color::RED(255, 0, 0);
Color Color::GREEN(0, 255, 0);
Color Color::BLUE(0, 0, 255);

Color::Color(uint8_t r_, uint8_t g_, uint8_t b_):
    r(r_), g(g_), b(b_) {}

Color::Color():
    r(0), g(0), b(0) {}


Color const &Color::operator*(float a)
{
    r = r * a / 255.;
    g = g * a / 255.;
    b = b * a / 255;
}

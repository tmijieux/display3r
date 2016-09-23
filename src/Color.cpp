#include <sstream>
#include "display3r/Color.hpp"
#include "display3r/Util.hpp"

using namespace display3r;

Color Color::RED(255, 0, 0);
Color Color::GREEN(0, 255, 0);
Color Color::BLUE(0, 0, 255);
Color Color::BLACK(0, 0, 0);
Color Color::WHITE(255, 255, 255);

Color Color::operator*(float scale) const
{
    return Color(
        scale * r,
        scale * g,
        scale * b
    );
}

static inline unsigned char sum(unsigned int a, unsigned int b)
{
    return (unsigned char) MIN(255, a+b);
}

Color Color::operator+(Color const &o) const
{
    return Color(
        sum(r, o.r),
        sum(g, o.g),
        sum(b, o.b)
    );
}

void Color::Filter(Color const &o)
{
    Color c;
    r = r * o.r / 255.;
    g = g * o.g / 255.;
    b = b * o.b / 255.;
}

static inline unsigned char prod(unsigned int a, unsigned int b)
{
    return (unsigned char) (a * b / 255);
}

Color Color::operator*(Color const &o) const
{
    return Color(
        prod(r, o.r),
        prod(g, o.g),
        prod(b, o.b)
    );
}

void Color::Average(Color const &o, unsigned char scale)
{
    r = r + (o.r - r) * scale / 255;
    g = g + (o.g - g) * scale / 255;
    b = b + (o.b - b) * scale / 255;
}

Color Color::FromString(std::string const &s)
{
    std::stringstream ss(s);
    ss.imbue(std::locale("C"));
    int r, g, b;
    ss >> r >> g >> b;
	return Color((uint8_t)r, (uint8_t)g, (uint8_t)b);
}

Color Color::Interpolate(
    Color const &A, Color const &B, Color const &C,
    float a, float b, float c)
{
    return Color(
        a*A.r + b*B.r + c*C.r,
        a*A.g + b*B.g + c*C.g,
        a*A.b + b*B.b + c*C.b
    );
}


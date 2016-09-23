#ifndef DISPLAY3R_OBJ_2D_H
#define DISPLAY3R_OBJ_2D_H

#include <glm/glm.hpp>

namespace display3r {

using glm::ivec2;
using glm::vec3;
using glm::vec2;

struct Pixel {
    ivec2 pos;
    float depth;
    Color light;
    vec2 texcoord;

    Pixel(const ivec2& A, float Depth, Color const &Light, vec2 const &Texcoord):
        pos(A), depth(Depth), light(Light), texcoord(Texcoord)
    {
    }
};

struct Line {
    ivec2 A, B;
    float dA, dB;
};

struct Triangle {
    ivec2 A, B, C; // 2D coords
    float dA, dB, dC; // depth
    Color lA, lB, lC; // lights
    vec2 U, V, W; // tex coords
};


};

#endif // DISPLAY3R_OBJ_2D_H

#ifndef FACE_H
#define FACE_H

#include <glm/glm.hpp>
#include "Vertex.hpp"

namespace display3r {

using namespace glm;

struct Face {
    Vertex A;
    Vertex B;
    Vertex C;
    Face(Vertex const& a, Vertex const& b, Vertex const& c): A(a),B(b),C(c) {}
    vec3 ComputeNormal() const;
};

struct Segment {
    vec3 A;
    vec3 B;
    Segment(vec3 const& a, vec3 const& b): A(a), B(b) {}
};





};

#endif //FACE_H

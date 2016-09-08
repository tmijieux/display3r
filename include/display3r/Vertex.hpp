#ifndef DISPLAY3R_VERTEX_H
#define DISPLAY3R_VERTEX_H

#include <glm/glm.hpp>

using namespace glm;

namespace display3r {

struct Vertex {
    vec3 m_position;
    vec3 m_normal;
    vec2 m_texcoord;
    Vertex();
    Vertex(vec3 const&, vec3 const&, vec2 const&);
};


};

#endif // DISPLAY3R_VERTEX_H


#include "display3r/Vertex.hpp"

using display3r::Vertex;

Vertex::Vertex():
    m_position(0.), m_normal(0.), m_texcoord(0.)
{
}

Vertex::Vertex(vec3 const& p, vec3 const& n, vec2 const& t):
    m_position(p), m_normal(n), m_texcoord(t)
{
}


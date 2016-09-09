#include <math.h>

#include "display3r/Color.hpp"
#include "display3r/Renderer.hpp"
#include "display3r/Window.hpp"
#include "display3r/Object3D.hpp"
#include "display3r/Object2D.hpp"
#include "display3r/Light.hpp"

using display3r::Renderer;
using display3r::Color;
using display3r::Light;

#define CUBE(x) ((x)*(x)*(x))

#define FORMAT_SCALE(scale) ((scale) >=0 ? 0.f : ((scale)<=-1 ? 1 : -(scale)))

Color Light::Compute(vec3 const &A, vec3 const &nA)
{
    Color c = Color::BLACK;
    vec3 OA = A - m_position;
    float dOA = glm::length(OA);
    float scale, angle = acos(dot(m_direction, OA) / dOA);

    if (m_inner < 0) {
        scale = FORMAT_SCALE(m_intensity * dot(m_direction, nA));
        c = m_color * scale;
    } else if (angle < m_inner) {
        scale = FORMAT_SCALE(m_intensity * dot(OA, nA) / CUBE(dOA));
        c = m_color * scale;
    } else if (angle < m_outer) {
        scale = FORMAT_SCALE( m_intensity *
                              ((angle-m_outer) / (m_inner-m_outer)) *
                              dot(OA, nA) / CUBE(dOA)  );
        c = m_color * scale;
    }
    return c;
}

Color Renderer::ComputeLight(vec3 const &pos, vec3 const &normal)
{
    if (m_lights == NULL)
        return Color::WHITE;

    Color c = Color::BLACK;
    for (auto &light : *m_lights)
        c = c + light.Compute(pos, normal);
    return c;
}

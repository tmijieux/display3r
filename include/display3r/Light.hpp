#ifndef DISPLAY3R_LIGHT_H
#define DISPLAY3R_LIGHT_H

#include <glm/glm.hpp>
#include "display3r/Color.hpp"

namespace display3r {

using glm::ivec2;
using glm::vec3;
using glm::vec2;

class Config;

class Light {
public:
    Light();
    Light(std::string const&, Config const&);

    Color Compute(vec3 const &A, vec3 const &nA);

private:
    vec3 m_position;
    vec3 m_direction;
    float m_intensity;
    float m_inner; //radian
    float m_outer; //radian
    Color m_color;
};

};
#endif //DISPLAY3R_LIGHT_H

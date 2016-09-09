#include <string>
#include "display3r/Light.hpp"
#include "display3r/Color.hpp"
#include "display3r/Util.hpp"

using display3r::Light;
using std::string;

Light::Light():
    m_position(1.0f, 1.0f, 1.0f),
    m_direction(1.0f, 1.0f, 1.0f),
    m_intensity(1.0f),
    m_inner(30),
    m_outer(40),
    m_color(Color::WHITE)
{
}

Light::Light(std::string const &name, Config const &conf)
{
    std::string id = "light."+name+".";
    m_position = ParseVec3(conf[id+"position"].as<string>());
    m_direction = ParseVec3(conf[id+"direction"].as<string>());
    m_intensity = conf[id+"intensity"].as<float>();
    m_inner = conf[id+"inner"].as<float>();
    m_inner = conf[id+"outer"].as<float>();
    m_color = Color::FromString(conf[id+"intensity"].as<string>());
}

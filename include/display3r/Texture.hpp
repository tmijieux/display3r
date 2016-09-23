#ifndef DISPLAY3R_TEXTURE_H
#define DISPLAY3R_TEXTURE_H

#include <SDL.h>
#include <string>
#include <stdint.h>
#include <glm/glm.hpp>
#include "display3r/Color.hpp"

namespace display3r {

using glm::ivec2;
using glm::vec3;
using glm::vec2;

class Texture {
public:
    Texture(std::string const &filename);
    ~Texture();
    static Texture *LoadTexture(std::string const &filename);
    Color GetColor(vec2 const &P);

private:
    SDL_Surface *m_surface;
    int m_width;
    int m_height;
    int m_bpp;
    int m_pitch;
};


};

#endif //DISPLAY3R_TEXTURE_H

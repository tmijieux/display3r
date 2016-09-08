#ifndef TEXTURE_H
#define TEXTURE_H

#include <SDL.h>
#include <string>
#include <stdint.h>
#include <glm/glm.hpp>
#include "display3r/Color.hpp"

using namespace glm;

namespace display3r {

class Texture {
public:
    Texture(std::string const &filename);
    ~Texture();
    static Texture *LoadTexture(std::string const &filename);
    Color GetColor(ivec2 P);
    
private:
    SDL_Surface *m_surface;
    int m_width;
    int m_height;
    uint32_t const *GetPixelPointer(ivec2 P);
};


};

#endif //TEXTURE_H

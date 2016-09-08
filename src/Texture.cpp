#include <iostream>
#include <string>

#include "display3r/Texture.hpp"

using std::string;
using display3r::Texture;
using display3r::Color;


Texture *Texture::LoadTexture(std::string const&filename)
{
    return new Texture(filename);
}


Texture::Texture(string const &filename):
    m_surface(NULL)
{
    m_surface = SDL_LoadBMP(filename.c_str());
    if (m_surface == NULL)
	std::cerr <<  SDL_GetError() << std::endl;
    else
	std::cout << "Texture successfully loaded" << std::endl;
    
    m_width = m_surface->format->BitsPerPixel/8 * m_surface->w;
    m_height = m_surface->h;
}

uint32_t const *Texture::GetPixelPointer(ivec2 P)
{
    return ((uint32_t*)m_surface->pixels) + P.x*m_width + P.y*m_height*m_width;
}

Color Texture::GetColor(ivec2 P)
{
    Color c;
    uint32_t const *p = GetPixelPointer(P);
    SDL_GetRGB(*p, m_surface->format, &c.r, &c.g, &c.b);
    return c;
}

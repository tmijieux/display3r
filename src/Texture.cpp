#include <iostream>
#include <string>

#include "display3r/Texture.hpp"
#include "display3r/Exception.hpp"
#include "display3r/Util.hpp"

using namespace std;
using display3r::Texture;
using display3r::Color;

Texture *Texture::LoadTexture(std::string const &filename)
{
    if (filename == "")
        return NULL;

    Texture *texture = NULL;
    try {
        texture = new Texture(filename);
    } catch (std::exception &e) {
        cout << "Cannot load Texture '"<< filename <<"'." << endl;
        cout << "'" << e.what() << "'" << endl;
        return NULL;
    }
    return  texture;
}

Texture::Texture(string const &filename):
    m_surface(NULL)
{
    m_surface = SDL_LoadBMP(filename.c_str());
    if (m_surface == NULL)
	throw display3r::exception(SDL_GetError());

    m_width = m_surface->w;
    m_height = m_surface->h;
    m_bpp = (int)m_surface->format->BytesPerPixel;
    m_pitch = (int)m_surface->pitch;
    cout << "Pitch: " << (int)m_pitch << endl;
}

Color Texture::GetColor(vec2 const &P)
{
    Color c;
    uint32_t const *p;
    p = (uint32_t*)((char*)m_surface->pixels +
                    (int)(P.x*m_width)*m_bpp + (int)(P.y*m_height)*m_pitch);
    uint32_t v = 0;
    memcpy(&v, p , m_bpp);
    SDL_GetRGB(v, m_surface->format, &c.r, &c.g, &c.b);

    return c;
}

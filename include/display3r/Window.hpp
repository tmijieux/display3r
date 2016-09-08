#ifndef WINDOW_H
#define WINDOW_H

#include <SDL.h>
#include <glm/glm.hpp>

#include "display3r/Renderer.hpp"
#include "display3r/Color.hpp"

using namespace glm;

namespace display3r {

class Scene;
class Config;
struct Event;

class Window {
public:
    
    bool IsValidCoordinate(ivec2 const& coord);
    virtual void SetPixel(ivec2 const &coord, Color const &color) = 0;
    virtual int GetWidth() = 0;
    virtual int GetHeight() = 0;
    virtual void Clear() = 0;
    virtual void Update() = 0;
    virtual bool PollEvent(Event &) = 0;
    
    bool HandleEvents(Scene &scene, Renderer &renderer);

    static Window *MakeWindow(Config const&);
    
protected:
    Window(int window, int height, Color const & bg);
    int m_width, m_height;
    Color m_bgColor;
};


class SDL_Window : public Window {
public:
    SDL_Window(int width, int height, Color const &bg);
    
    int GetWidth() override;
    int GetHeight() override;
    void Clear() override;
    void Update() override;
    bool PollEvent(Event&) override;
    void SetPixel(ivec2 const &coord, Color const &color) override;
    Color GetPixel(ivec2 const &coord);

private:
    static bool SDL_Inited;
    static void InitSDLOnce();
    
private:
    ::SDL_Window *m_window;
    ::SDL_Renderer *m_renderer;
};

struct Event {
    enum EventType {
        QUIT,
        ROTATE,
        TRANSLATE,
        HIDE_FRAME, // toggle hide frame
        DRAW,
        RESIZE,
        LOAD,
        UNLOAD,
    };
    EventType type;
    Direction direction;
    ::display3r::Renderer::DrawState drawState;
    int width;
    int height;
};


}; // end namespace display3r

#endif //WINDOW_H

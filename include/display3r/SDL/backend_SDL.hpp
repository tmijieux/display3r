#ifndef DISPLAY3R_SDL_WINDOW_H
#define DISPLAY3R_SDL_WINDOW_H

#include <SDL.h>
#include "display3r/Window.hpp"
#include "display3r/WindowMaker.hpp"

class backend_SDL_factory : public display3r::IWindowMaker
{
public:
    display3r::Window *MakeWindow(
        int width, int height, display3r::Color c) override;
private:
    static bool SDL_Inited;
    static void InitSDLOnce();
};

class backend_SDL : public display3r::Window {
    typedef display3r::Event Event;
    typedef display3r::Color Color;
public:
    backend_SDL(int width, int height, Color const &bg);
    ~backend_SDL() override;

    int GetWidth() override;
    int GetHeight() override;
    void Clear() override;
    void Update() override;
    bool PollEvent(Event&) override;
    void WaitEvent(Event&) override;
    void SetPixel(ivec2 const &coord, Color const &color) override;
    Color GetPixel(ivec2 const &coord) override;

private:
    void ParseEvent(Event &event, SDL_Event &sdl_event);

    void handleMouseButtonUpEvent(SDL_Event &sdl_event);
    void handleMouseButtonDownEvent(SDL_Event &sdl_event);
    void handleMouseMotionEvent(Event &event, SDL_Event &sdl_event);
    void handleMouseWheelEvent(Event &event, SDL_Event &sdl_event);
    void handleKeyUpEvent(Event &event, SDL_Event &sdl_event);
    void handleKeyDownEvent(Event &event, SDL_Event &sdl_event);
    void handleWindowEvent(Event &event, SDL_Event &sdl_event);

private:
    SDL_Window *m_window;
    SDL_Renderer *m_renderer;
    // SDL_Texture *m_texture;
    // std::vector<uint32_t> m_softSurface;

    bool _rightClickDown;
    int _mouseX;
    int _mouseY;
};

#endif // DISPLAY3R_SDL_WINDOW_H

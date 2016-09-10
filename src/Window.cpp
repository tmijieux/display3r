#include <iostream>
#include "display3r/Window.hpp"
#include "display3r/Scene.hpp"
#include "display3r/Renderer.hpp"
#include "display3r/Config.hpp"

using namespace std;
using display3r::Window;
using display3r::Renderer;

Window::Window(int width, int height, Color const &bg):
    m_width(width), m_height(height), m_bgColor(bg)
{
}

bool Window::HandleEvents(Scene &scene, Renderer &renderer)
{
    Event event;
    while (!this->PollEvent(event))
    { /* WAIT */ }

    do {
        switch (event.type) {
        case Event::QUIT:
            return true;
        case Event::TRANSLATE:
            scene.TranslateCamera(event.direction);
            break;
        case Event::ROTATE:
            scene.RotateCamera(event.direction);
            break;
        case Event::DRAW:
            renderer.SetDrawState(event.drawState);
            break;
        case Event::RESIZE:
            scene.NotifyResize(event.width, event.height);
            break;
        case Event::HIDE_FRAME:
            scene.ToggleFrame();
            break;
        case Event::LOAD:
            scene.AskSolid();
            break;
        case Event::UNLOAD:
            scene.RemoveSolid();
            break;
        default:
            break;
        };
    } while (this->PollEvent(event));
    return false;
}

Window *Window::MakeWindow(Config const &conf)
{
    return new SDL_Window(conf.WindowWidth,
                          conf.WindowHeight,
                          conf.WindowBackground);
}

#include <iostream>

#include "display3r/Window.hpp"
#include "display3r/Scene.hpp"
#include "display3r/Renderer.hpp"


using display3r::Window;
using display3r::Renderer;

Window::Window(int width, int height, Color const &bg):
    m_width(width), m_height(height), m_bgColor(bg)
{
}

bool Window::HandleEvents(Scene &scene, Renderer &renderer)
{
    Event event;
    this->WaitEvent(event);

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
            renderer.ToggleDrawState(event.drawState);
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

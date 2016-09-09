#include <SDL.h>
#include "display3r/Window.hpp"
#include "display3r/Scene.hpp"
#include "display3r/Renderer.hpp"
#include "display3r/Exception.hpp"

namespace display3r {

bool SDL_Window::SDL_Inited = false;

int SDL_Window::GetWidth()
{
    SDL_GetWindowSize(m_window, &m_width, &m_height);
    return m_width;
}

int SDL_Window::GetHeight()
{
    SDL_GetWindowSize(m_window, &m_width, &m_height);
    return m_height;
}

void SDL_Window::InitSDLOnce()
{
    if (SDL_Inited)
        return;

    if (SDL_Init(SDL_INIT_VIDEO) == -1)
        throw exception(string("Error SDL_Init")+SDL_GetError());
    SDL_Inited = true;
}

void SDL_Window::Clear()
{
    SDL_SetRenderDrawColor(m_renderer, m_bgColor.r, m_bgColor.g, m_bgColor.b, 255);
    SDL_RenderClear(m_renderer);
}

void SDL_Window::Update()
{
    SDL_RenderPresent(m_renderer);
}

void SDL_Window::SetPixel(ivec2 const &coord, Color const &c)
{
    SDL_SetRenderDrawColor(m_renderer, c.r, c.g, c.b, 255);
    SDL_RenderDrawPoint(m_renderer, coord.x, coord.y);
}

Color SDL_Window::GetPixel(ivec2 const & coord)
{
    Color c;
    SDL_Rect rect;
    uint32_t pixel;

    rect.x = coord.x; rect.y = coord.y;
    rect.w = 1; rect.h = 1;

    SDL_RenderReadPixels(m_renderer, &rect, SDL_PIXELFORMAT_RGBA8888, &pixel, 0);

    c.r = (pixel >> 24) & 0xFF;
    c.g = (pixel >> 16) & 0xFF;
    c.b = (pixel >> 8) & 0xFF;
    return c;
}

SDL_Window::SDL_Window(int width, int height, Color const &bg):
    Window(width, height, bg),
    _rightClickDown(false),
    _mouseX(0), _mouseY(0)
{
    InitSDLOnce();

    m_window = SDL_CreateWindow(
        "3Displayer",  SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        width, height, SDL_WINDOW_RESIZABLE);
    if (m_window == NULL)
        throw exception(string("Could not create window: ")+SDL_GetError());

    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_SOFTWARE);
}

bool SDL_Window::PollEvent(Event &event)
{
    SDL_Event sdl_event;
    if (!SDL_PollEvent(&sdl_event))
        return false;

    event.type = Event::OTHER;
    switch (sdl_event.type) {
    case SDL_QUIT:
        event.type = Event::QUIT;
        break;
    case SDL_KEYDOWN:
        handleKeyDownEvent(event, sdl_event);
        break;
    case SDL_KEYUP:
        handleKeyUpEvent(event, sdl_event);
        break;
    case SDL_MOUSEMOTION:
        handleMouseMotionEvent(event, sdl_event);
        break;
    case SDL_MOUSEBUTTONUP:
        handleMouseButtonUpEvent(sdl_event);
        break;
    case SDL_MOUSEBUTTONDOWN:
        handleMouseButtonDownEvent(sdl_event);
        break;
    case SDL_MOUSEWHEEL:
        handleMouseWheelEvent(event, sdl_event);
        break;
    case SDL_WINDOWEVENT:
        handleWindowEvent(event, sdl_event);
        break;
    default:
        break;
    }
    return true;
}

void SDL_Window::handleMouseMotionEvent(Event &event, SDL_Event &sdl_event)
{
    if (_rightClickDown) {
        event.type = Event::ROTATE;

	if (sdl_event.motion.x < _mouseX)
            event.direction = LEFT;
	else if (sdl_event.motion.x > _mouseX)
            event.direction = RIGHT;
	else if (sdl_event.motion.y < _mouseY)
            event.direction = UP;
	else if (sdl_event.motion.y > _mouseY)
            event.direction = DOWN;
    }
    _mouseX = sdl_event.motion.x;
    _mouseY = sdl_event.motion.y;
}

void SDL_Window::handleMouseButtonUpEvent(SDL_Event &sdl_event)
{
    switch (sdl_event.button.button) {
    case SDL_BUTTON_RIGHT:
	_rightClickDown = false;
        break;
    default:
        break;
    }
}

void SDL_Window::handleMouseButtonDownEvent(SDL_Event &sdl_event)
{
    switch (sdl_event.button.button) {
    case SDL_BUTTON_RIGHT:
	_rightClickDown = true;
        break;
    default:
        break;
    }
}

void SDL_Window::handleMouseWheelEvent(Event &event, SDL_Event &sdl_event)
{
    event.type = Event::TRANSLATE;
    if (sdl_event.wheel.y > 0)
        event.direction = FORWARD;
    else if (sdl_event.wheel.y < 0)
        event.direction = BACKWARD;
    else
        event.type = Event::OTHER;
}

void SDL_Window::handleKeyDownEvent(Event &event, SDL_Event &sdl_event)
{
    switch (sdl_event.key.keysym.sym) {
    case SDLK_q:
        event.type = Event::TRANSLATE;
        event.direction = LEFT;
	break;
    case SDLK_d:
        event.type = Event::TRANSLATE;
        event.direction = RIGHT;
	break;
    case SDLK_e:
        event.type = Event::TRANSLATE;
        event.direction = UP;
	break;
    case SDLK_a:
        event.type = Event::TRANSLATE;
        event.direction = DOWN;
	break;
    case SDLK_z:
        event.type = Event::TRANSLATE;
        event.direction = FORWARD;
	break;
    case SDLK_s:
        event.type = Event::TRANSLATE;
        event.direction = BACKWARD;
	break;
    case SDLK_LEFT:
        event.type = Event::ROTATE;
        event.direction = LEFT;
	break;
    case SDLK_RIGHT:
        event.type = Event::ROTATE;
        event.direction = RIGHT;
	break;
    case SDLK_UP:
        event.type = Event::ROTATE;
        event.direction = UP;
	break;
    case SDLK_DOWN:
        event.type = Event::ROTATE;
        event.direction = DOWN;
	break;
    case SDLK_ESCAPE:
        event.type = Event::QUIT;
	break;
    default:
        break;
    }
}

void SDL_Window::handleKeyUpEvent(Event &event, SDL_Event &sdl_event)
{
    switch (sdl_event.key.keysym.sym) {
    case SDLK_o:
        event.type = Event::DRAW;
        event.drawState = Renderer::DrawState::FACE;
	break;
    case SDLK_w:
        event.type = Event::DRAW;
        event.drawState = Renderer::DrawState::WIREFRAME;
	break;
    case SDLK_n:
        event.type = Event::DRAW;
        event.drawState = Renderer::DrawState::NORMAL;
	break;
    case SDLK_v:
        event.type = Event::DRAW;
        event.drawState = Renderer::DrawState::VERTEX;
	break;
    case SDLK_f:
        event.type = Event::HIDE_FRAME;
	break;
    case SDLK_l:
        event.type = Event::LOAD;
	break;
    case SDLK_u:
        event.type = Event::UNLOAD;
	break;
    default:
        break;
    }
}

void SDL_Window::handleWindowEvent(Event &event, SDL_Event &sdl_event)
{
    switch(sdl_event.window.event) {
    case SDL_WINDOWEVENT_RESIZED:
        event.type = Event::RESIZE;
        event.width = sdl_event.window.data1;
        event.height = sdl_event.window.data2;
        break;
    default:
        break;
    }
}

}; // end namespace display3r


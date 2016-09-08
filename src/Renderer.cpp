#include <tuple>
#include <utility>

#include "display3r/Renderer.hpp"
#include "display3r/Window.hpp"


using display3r::Renderer;
using display3r::ZBuffer;

Renderer::Renderer(Camera const *camera, Window *window):
    m_camera(camera), m_window(window)
{
    Resize(m_window->GetWidth(), m_window->GetHeight());
}

void Renderer::Clear()
{
    m_window->Clear();
    m_zbuf.Clear();
}

void Renderer::Resize(int width, int height)
{
    m_zbuf.Resize(width, height);
}

void ZBuffer::Clear()
{
    union { int i; float f; } v = {.f = -1.0 };
    memset(&m_buf[0], v.i, m_buf.size());
}

void ZBuffer::Resize(int width, int height)
{
    m_buf.resize(width*height);
}


void Renderer::SetColor(Color const &drawColor)
{
    m_drawColor = drawColor;
}

void Renderer::SetDrawState(Renderer::DrawState drawState)
{
    m_drawState = drawState;
}

void Renderer::PushState()
{
    m_states.push(std::make_pair(m_drawState, m_drawColor));
}

void Renderer::PopState()
{
    std::tie(m_drawState, m_drawColor) = m_states.top();
    m_states.pop();
}


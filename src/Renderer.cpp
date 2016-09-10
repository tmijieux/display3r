#include <string>
#include <tuple>
#include <utility>

#include "display3r/Renderer.hpp"
#include "display3r/Window.hpp"
#include "display3r/Color.hpp"

using namespace std;
using display3r::Renderer;
using display3r::ZBuffer;

Renderer::Renderer(Window *window, Color const &untextured):
    m_lens(NULL),
    m_window(window),
    m_zbuf(NULL),
    m_drawState(FACE),
    m_drawColor(Color::BLUE),
    m_untexturedColor(untextured)
{
}

void Renderer::BindTexture(Texture *texture)
{
    m_texture = texture;
}

void Renderer::BindLights(vector<Light> *lights)
{
    m_lights = lights;
}

void Renderer::SetLens(Lens *lens)
{
    m_lens = lens;
    m_nearplan = lens->GetNearPlan();
    m_width = lens->GetWindowWidth();
    m_height = lens->GetWindowHeight();
    m_wfov = lens->GetWFov();
    m_hfov = lens->GetHFov();
    m_zbuf = lens->GetZBuffer();

    // lens->ConfigureZBuffer(m_zbuf);
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
    m_states.push(make_pair(m_drawState, m_drawColor));
}

void Renderer::PopState()
{
    tie(m_drawState, m_drawColor) = m_states.top();
    m_states.pop();
}

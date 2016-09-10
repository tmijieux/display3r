#include "display3r/Drawable.hpp"

using display3r::Drawable;

Drawable::Drawable():
    m_hidden(false)
{
}

void Drawable::Draw(Renderer &renderer)
{
    if (m_hidden)
        return;

    this->DrawHandler(renderer);
}

void Drawable::SetHidden(bool value)
{
    m_hidden = value;
}

void Drawable::ToggleVisibility()
{
    m_hidden = !m_hidden;
}

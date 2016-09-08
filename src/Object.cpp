#include "display3r/Object.hpp"

using display3r::Object;

Object::Object():
    m_hidden(false)
{
}

void Object::Draw(Renderer &renderer)
{
    if (m_hidden)
        return;

    this->DrawHandler(renderer);
}

void Object::SetHidden(bool value)
{
    m_hidden = value;
}

void Object::ToggleVisibility()
{
    m_hidden = !m_hidden;
}

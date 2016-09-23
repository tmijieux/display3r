#include <algorithm>
#include "display3r/ZBuffer.hpp"

using namespace display3r;


ZBuffer::ZBuffer():
    m_width(0)
{
}

void ZBuffer::Clear()
{
    std::fill(m_buf.begin(), m_buf.end(), -1.0f);
}

void ZBuffer::Resize(int width, int height)
{
    m_width = width;
    m_buf.resize(width*height);
}

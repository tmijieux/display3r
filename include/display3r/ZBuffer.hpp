#ifndef DISPLAY3R_ZBUFFER_H
#define DISPLAY3R_ZBUFFER_H

#include <vector>
#include <glm/glm.hpp>

namespace display3r {

using glm::ivec2;
using glm::vec3;
using glm::vec2;

class ZBuffer {
public:
    ZBuffer();
    inline float &operator[](ivec2 i) { return m_buf[i.y*m_width + i.x]; }

    void Clear();
    void Resize(int width, int height);

private:
    int m_width;
    std::vector<float> m_buf;
};


};

#endif //DISPLAY3R_ZBUFFER_H

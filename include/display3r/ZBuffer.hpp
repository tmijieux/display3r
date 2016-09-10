#ifndef ZBUFFER_H
#define ZBUFFER_H

#include <vector>
#include <glm/glm.hpp>

namespace display3r {

using namespace glm;

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

#endif //ZBUFFER_H

#include <vector>
#include <string>
#include <sstream>

#include <glm/glm.hpp>
#include "display3r/Util.hpp"

using namespace std;

namespace display3r {

glm::vec3 ParseVec3(string const &s)
{
    stringstream ss(s);
    glm::vec3 v;
    ss.imbue(locale("C"));
    ss >> v.x >> v.y >> v.z;
    return v;
}

};

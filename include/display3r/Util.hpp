#ifndef UTIL_H
#define UTIL_H

#include <glm/glm.hpp>



#ifdef MAX
#undef MAX
#endif
#define MAX(x,y) ((x) < (y) ? (y) : (x))

#ifdef MIN
#undef MIN
#endif
#define MIN(x,y) ((x) < (y) ? (x) : (y))

namespace display3r {

using namespace glm;

vec3 ParseVec3(std::string const &);
};


#endif //UTIL_H

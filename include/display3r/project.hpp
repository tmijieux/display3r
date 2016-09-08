#ifndef PROJECT_H
#define PROJECT_H

#include <glm/glm.hpp>
#include <SDL.h>
#include "display3r/Color.hpp"
#include "display3r/Vertex.hpp"
#include "display3r/Face.hpp"

using namespace glm;

namespace display3r {

// return the vector between user.position and the intersection of
// (AB) and the NEARPLAN


vec3 ProjectPoint(vec3 const& A, vec3 const& B);
    
// return the projection of a vector between user.position
// and A, knowing the depth of A
ivec2 ProjectCoord(vec3 const& OA, float depth);
	
void ProjectVertex(Vertex const& v);
void ProjectSegment(Vertex const& A, Vertex const& B);
void ProjectFace(Face const &F);


};

#endif //PROJECT_H

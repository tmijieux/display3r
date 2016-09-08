#ifndef SOLID_H
#define SOLID_H

#include <SDL.h>
#include <string>
#include <vector>
#include <fstream>

namespace display3r { class Solid; }


#include "display3r/Face.hpp"
#include "display3r/Color.hpp"
#include "display3r/Object.hpp"
#include "display3r/Texture.hpp"
#include "display3r/Equation.hpp"

//#include "equation.hpp"
namespace display3r {

class Renderer;

using std::string;
using std::ifstream;
using std::vector;

class Solid : public Object {


public:
    Solid(string filepath, Texture*);
/*    Solid(Equation const&, Texture); */
    
private:
    void DrawHandler(Renderer &renderer) override;
    
private:
    void LoadOBJ(ifstream &file);

    void ParseVertex(vector<string> const &s, vector<vec3> &vertex);
    void ParseNormal(vector<string> const &s, vector<vec3> &normal);
    void ParseTexCoord(vector<string> const &s, vector<vec2> &texcoord);
    Vertex ParseVertexIndex(string rep,
                            vector<vec3>const &vertex,
                            vector<vec3>const &normal,
                            vector<vec2>const &texcoord);
    
    void ParseFace(vector<string>  const &s,
                   vector<vec3>const &v/*ertex*/,
                   vector<vec3>const &n/*ormal*/,
                   vector<vec2>const &t/*excoord*/);
      

    std::vector<Face> m_faces;
    Texture *m_texture;
};

};

#endif //SOLID_H

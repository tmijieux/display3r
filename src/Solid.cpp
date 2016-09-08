#include <iostream>
#include <sstream>
#include <locale>
#include <boost/algorithm/string.hpp>

#include "display3r/Solid.hpp"
#include "display3r/Exception.hpp"
#include "display3r/Renderer.hpp"

using std::string;
using std::ifstream;
using std::vector;
using display3r::Solid;
using display3r::Vertex;
using display3r::Face;

namespace display3r {

static inline double strtod(std::string const &s)
{
    std::istringstream istr(s);
    double d;
    
    istr.imbue(std::locale("C"));
    istr >> d;
    return d;
}
static inline int atoi(std::string const &s)
{
    return ::atoi(s.c_str());
}

}

void Solid::ParseVertex(vector<string> const &s, vector<vec3> &vertex)
{
    vec3 v;
    if (s.size() != 3)
        throw exception("invalid vertex");
    v.x = strtod(s[0]);
    v.y = strtod(s[1]);
    v.z = strtod(s[2]);
    
    vertex.push_back(v);
}

void Solid::ParseNormal(vector<string> const &s, vector<vec3> &normal)
{
    vec3 n;
    if (s.size() != 2)
        throw exception("invalid normal");
    n.x = strtod(s[0]);
    n.y = strtod(s[1]);
    n.z = strtod(s[2]);
    
    normal.push_back(n);
}

void Solid::ParseTexCoord(vector<string> const &s, vector<vec2> &texcoord)
{
    vec2 t;
    if (s.size() != 3)
        throw exception("invalid texcoord");
    t.x = strtod(s[0]);
    t.y = strtod(s[1]);
    
    texcoord.push_back(t);
}

Vertex Solid::ParseVertexIndex(string rep,
                               vector<vec3>const &vertex,
                               vector<vec3>const &normal,
                               vector<vec2>const &texcoord)
{
    Vertex v;
    vector<string> s;
    boost::trim(rep);
    boost::split(s, rep, boost::is_any_of("/"));
    
    if (!s.size() || s.size() > 3)
        throw exception("bad face element");
    
    v.m_position = vertex[atoi(s[0])-1];
    if (s.size() >= 2)
        v.m_texcoord = texcoord[atoi(s[1])-1];
    if (s.size() == 3)
        v.m_normal = normal[atoi(s[2])-1];
    return v;
}

void Solid::ParseFace(vector<string> const &s,
                      vector<vec3>const &v/*ertex*/,
                      vector<vec3>const &n/*ormal*/,
                      vector<vec2>const &t/*excoord*/)
{
    if (s.size() < 3)
        throw exception("invalid face");

    Face f(ParseVertexIndex(s[0], v, n, t),
           ParseVertexIndex(s[1], v, n, t),
           ParseVertexIndex(s[2], v, n, t));
    m_faces.push_back(f);

    // triangle fan if more than 3 points:
    Vertex last = f.C;
    for (unsigned i = 3; i < s.size(); ++i) {
        Face g(f.A, last, ParseVertexIndex(s[i], v, n, t));
        last = g.C;
        m_faces.push_back(g);
    }
}

void Solid::LoadOBJ(ifstream &file)
{
    m_faces.clear();
    std::vector<vec3> normals;
    std::vector<vec3> vertex;
    std::vector<vec2> texcoords;
    
    string line;
    while (getline(file, line)) {
        boost::trim(line);
        vector<string> s;
        boost::split(s, line, boost::is_any_of(" "));
        string const &type = s[0];

	if (type == "v")
            ParseVertex(s, vertex);
	else if (type == "vn")
            ParseNormal(s, normals);
	else if (type == "vt")
            ParseTexCoord(s, texcoords);
        else if (type == "f")
            ParseFace(s, vertex, normals, texcoords);
    }
}


Solid::Solid(string filepath, Texture *texture):
    m_texture(NULL)
{
    ifstream f(filepath);
    
    if (!f.is_open())
	throw exception(filepath+": "+strerror(errno)) ;

    m_texture = texture;
    
    LoadOBJ(f);
    std::cout << "Solid "<< filepath << " successfully loaded" << std::endl;
    f.close();
}

void Solid::DrawHandler(Renderer &renderer)
{
    for (auto &f : m_faces)
        renderer.DrawFace(f);
}


// Solid::Solid(Equation &eq, std::string texpath)
// {
//     float minS, maxS, minT, maxT;
//     int precisionS, precisionT;
//     char x[MAXLENGTH];
//     char y[MAXLENGTH];
//     char z[MAXLENGTH];
    
//     if (!initEquation(&minS, &maxS, &precisionS,
// 		      &minT, &maxT, &precisionT,
// 		      x, y, z, MAXLENGTH, eqName)) {
// 	fprintf(stderr, "Error loading equation\n");
// 	return NULL;
//     } else 
// 	printf("Equation successfully initialized\n");

//     Solid *solid = malloc(sizeof(Solid));
//     int p, f = 0;
//     float s = minS, t = minT;
//     float ds = (maxS - minS) / (precisionS - 1);
//     float dt = (maxT - minT) / (precisionT - 1);

//     solid->numVertices = precisionS * precisionT;
//     solid->numNormals = solid->numVertices;
//     solid->numCoords = 4;
//     solid->numFaces = 2 * (precisionS - 1) * (precisionT - 1);

//     solid->vertices = malloc(solid->numVertices * sizeof(Point));
//     solid->normals = malloc(solid->numNormals * sizeof(Point));
//     solid->coords = malloc(solid->numCoords * sizeof(Texture));
//     solid->faces = malloc(solid->numFaces * sizeof(Face));

//     setTexture(&solid->coords[0], 0., 0.);
//     setTexture(&solid->coords[1], 0., 1.);
//     setTexture(&solid->coords[2], 1., 0.);
//     setTexture(&solid->coords[3], 1., 1.);

//     if ((solid->texture = SDL_LoadBMP(texpath.c_str())) == NULL)
// 	fprintf(stderr, "%s\n", SDL_GetError());
    
//     if (solid->numVertices > 0)
// 	getValueFromEquation(x, y, z, s, t, &solid->vertices[0]);	

//     for (p = 0; p < solid->numVertices; p++) {
// 	Point *A;
// 	Point *B;
// 	Point *O = &solid->vertices[p];
// 	Point *normal = &solid->normals[p];
// 	Point u;
// 	Point v;
	
// 	printf("%d => s: %f, t: %f\n", p, s, t);
// 	printf("%d => x: %f, y: %f, z: %f\n", p, O->x, O->y, O->z);

//         if (p == solid->numVertices - 1) { // north-east
// 	    A = &solid->vertices[p - 1];
// 	    B = &solid->vertices[p - precisionS];
// 	} else if (p % precisionS == precisionS - 1) { // east
// 	    A = &solid->vertices[p + precisionS];
// 	    B = &solid->vertices[p - 1];

// 	    getValueFromEquation(x, y, z, s, t + dt, A);
// 	    s = minS;
// 	    t += dt;
// 	} else if (p >= (solid->numVertices - precisionS)) { // north
// 	    A = &solid->vertices[p - precisionS];
// 	    B = &solid->vertices[p + 1];

// 	    s += ds;
// 	} else if (p < precisionS) { // south
// 	    A = &solid->vertices[p + 1];
// 	    B = &solid->vertices[p + precisionS];
	    
// 	    getValueFromEquation(x, y, z, s + ds, t, A);
// 	    getValueFromEquation(x, y, z, s, t + dt, B);
// 	    s += ds;
// 	} else { // elsewhere
// 	    A = &solid->vertices[p + 1];
// 	    B = &solid->vertices[p + precisionS];
	    
// 	    getValueFromEquation(x, y, z, s, t + dt, B);
// 	    s += ds;
// 	}
// 	diffPoint(A, O, &u);
// 	diffPoint(B, O, &v);
// 	pointProduct(&u, &v, normal);
// 	normalizePoint(normal, normal);
//     }

//     p = 0;
//     while (f < solid->numFaces) {
// 	if (p % precisionS != precisionS - 1) {
// 	    solid->faces[f].vertices[0].point = p;
// 	    solid->faces[f].vertices[1].point = p + 1;
// 	    solid->faces[f].vertices[2].point = p + precisionS;
// 	    solid->faces[f].vertices[0].normal = p;
// 	    solid->faces[f].vertices[1].normal = p + 1;
// 	    solid->faces[f].vertices[2].normal = p + precisionS;
// 	    solid->faces[f].vertices[0].coord = 1;
// 	    solid->faces[f].vertices[1].coord = 3;
// 	    solid->faces[f].vertices[2].coord = 0;

// 	    f++;

// 	    solid->faces[f].vertices[0].point = p + precisionS;
// 	    solid->faces[f].vertices[1].point = p + 1;
// 	    solid->faces[f].vertices[2].point = p + 1 + precisionS;
// 	    solid->faces[f].vertices[0].normal = p + precisionS;
// 	    solid->faces[f].vertices[1].normal = p + 1;
// 	    solid->faces[f].vertices[2].normal = p + 1 + precisionS;
// 	    solid->faces[f].vertices[0].coord = 0;
// 	    solid->faces[f].vertices[1].coord = 3;
// 	    solid->faces[f].vertices[2].coord = 2;

// 	    f++;
// 	}
// 	p++;
//     }
//     solid->computeOrigin();
//     freeEquation();
//     printf("Equation successfully loaded\n");
//     return solid;
// }

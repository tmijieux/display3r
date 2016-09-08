#include <math.h>

#include "display3r/project.hpp"
#include "display3r/Color.hpp"
#include "display3r/Renderer.hpp"
#include "display3r/Window.hpp"

using display3r::Renderer;
using display3r::Color;

// return the vector between camera.O and the intersection of
// (AB) and the NEARPLAN
vec3 Renderer::ProjectPoint(const vec3& A, const vec3& B)
{
    vec3 AB = B - A;
    vec3 d = A - m_camera->O;
    float k = m_camera->nearplan - dot(m_camera->j, d) / dot(m_camera->j, AB);
    return A+AB*k - m_camera->O;
}

// return the Coord projection of a vector between scene.camera.O
// and A, knowing the depth of A
vec2 Renderer::ProjectCoord(vec3 const& OA, float depth)
{
    int height = m_window->GetHeight();
    int width = m_window->GetWidth();

    return vec2(
        width/(2.*tan(m_camera->wfov * M_PI / 360.))
        * dot(m_camera->i, OA) / depth + width / 2,

        -height/(2.*tan(m_camera->hfov * M_PI / 360.))
        * dot(m_camera->k, OA) / depth + height / 2);
}

void Renderer::DrawVertex(Vertex const &V)
{
    DrawVertex(V.m_position);
}

void Renderer::DrawVertex(vec3 const& A)
{
    vec3 OA = A - m_camera->O;
    float depthA = dot(m_camera->j, OA);
    vec2 p = ProjectCoord(OA, depthA);

    if (depthA > m_camera->nearplan)
	DrawPixel(Pixel(p, depthA));
}

void Renderer::DrawSegment(Segment const &S)
{
    DrawSegment(S.A, S.B);
}

void Renderer::DrawSegment(vec3 const &A, vec3 const &B)
{
    float nearplan = m_camera->nearplan;
    vec3 OA = A - m_camera->O;
    vec3 OB = B - m_camera->O;
    float depthA = dot(m_camera->j, OA);
    float depthB = dot(m_camera->j, OB);

    ivec2 t, u;
    if (depthA > nearplan &&  depthB > nearplan) {
	t = ProjectCoord(OA, depthA);
	u = ProjectCoord(OB, depthB);
    } else if (depthA < nearplan && depthB > nearplan) {
	vec3 AB = ProjectPoint(A, B);
	t = ProjectCoord(AB, nearplan);
	u = ProjectCoord(OB, depthB);
    } else if (depthA > nearplan && depthB < nearplan) {
	vec3 AB = ProjectPoint(A, B);
	t = ProjectCoord(OA, depthA);
	u = ProjectCoord(AB, nearplan);
    } else
	return;
    
    DrawLine(t, u, depthA, depthB);
}


void Renderer::DrawFace(Face const &f)
{
    DrawFace(f.A.m_position, f.B.m_position, f.C.m_position,
             f.A.m_texcoord, f.B.m_texcoord, f.C.m_texcoord,
             f.A.m_normal, f.B.m_normal, f.C.m_normal);
}

void Renderer::DrawFace(vec3 const &A, vec3 const &B, vec3 const &C,
                        vec2 const &U, vec2 const &V, vec2 const &W,
                        vec3 const &nA, vec3 const &nB, vec3 const &nC)
{
    float nearplan = m_camera->nearplan;
    vec3 O = m_camera->O;
    vec3 OA = A - O;
    vec3 OB = B - O;
    vec3 OC = C - O;

    OB = B - O;
    OC = C - O;
    float depthA = dot(m_camera->j, OA);
    float depthB = dot(m_camera->j, OB);
    float depthC = dot(m_camera->j, OC);
    
    int test = (depthA > nearplan) + (depthB > nearplan) + (depthC > nearplan);
    if (test == 0) {
        return;
    } else if (test == 3) {
	ivec2 a = ProjectCoord(OA, depthA);
	ivec2 b = ProjectCoord(OB, depthB);
	ivec2 c = ProjectCoord(OC, depthC);
	DrawTriangle(a, b, c, depthA, depthB, depthC,  U, V, W, nA, nB, nC);
    } else if (test == 1) {
	if (depthA > nearplan) {
            vec3 b = B - A;
            vec3 c = C - A;
	    float kB = (nearplan - depthA) / dot(m_camera->j, b);
	    float kC = (nearplan - depthA) / dot(m_camera->j, c);

            OB = A + b * kB - O;
            OC = A + c * kC - O;
	    ivec2 a = ProjectCoord(OA, depthA);
	    ivec2 bn = ProjectCoord(OB, nearplan);
	    ivec2 cn = ProjectCoord(OC, nearplan);

            vec2 UV = (V - U) * kB + U;
	    vec2 UW = (W - U) * kC + U;
            vec3 nAB = (nB - nA) * kB + nA;
            vec3 nAC = (nC - nA) * kC + nA;
            
	    DrawTriangle(a, bn, cn, depthA,
                         (depthB - depthA) * kB + depthA,
                         (depthC - depthA) * kC + depthA,
                         U, UV, UW,  nA, nAB, nAC);

	} else if (depthB > nearplan) {
	    vec3 c = C - B;
	    vec3 a = A - B;
	    float kC = (nearplan - depthB) / dot(m_camera->j, c);
	    float kA = (nearplan - depthB) / dot(m_camera->j, a);
            
            OC = B + c * kC - O;
            OA = B + a * kA - O;

	    ivec2 b = ProjectCoord(OB, depthB);
	    ivec2 cn = ProjectCoord(OC, nearplan);
	    ivec2 an = ProjectCoord(OA, nearplan);

            vec2 VW = (W - V) * kC + V;
            vec2 VU = (U - V) * kA + V;
            vec3 nBC = (nC - nB) * kC + nB;
            vec3 nBA = (nA - nB) * kA + nB;

	    DrawTriangle(b, cn, an,
                         depthB,
                         (depthC - depthB) * kC + depthB,
                         (depthA - depthB) * kA + depthB,
                         V, VW, VU,    nB, nBC, nBA);

	} else if (depthC > nearplan) {
	    vec3 a = A - C;
	    vec3 b = B - C;

	    float kA = (nearplan - depthC) / dot(m_camera->j, a);
	    float kB = (nearplan - depthC) / dot(m_camera->j, b);

            OA = C + a * kA - O;
            OB = C + b * kB - O;

	    ivec2 c =  ProjectCoord(OC, depthC);
	    ivec2 an = ProjectCoord(OA, nearplan);
	    ivec2 bn = ProjectCoord(OB, nearplan);

            vec2 WU = (U - V) * kA + W;
            vec2 WV = (V - W) * kB + W;
            vec3 nCA = (nA - nC) * kA + nC;
            vec3 nCB = (nB - nC) * kB + nC;

	    DrawTriangle(c, an, bn,
                         depthC,
                         (depthA - depthC) * kA + depthC,
                         (depthB - depthC) * kB + depthC,
                         W, WU, WV,  nC, nCA, nCB);
	}
    } else /* if (test == 2) */ {
	// Coord XY, XZ, Y, Z;
	if (depthA < nearplan) {
	    vec3 AB = B - A;
	    vec3 AC = C - A;

	    float kB = (nearplan - depthA) / dot(m_camera->j, AB);
	    float kC = (nearplan - depthA) / dot(m_camera->j, AC);

            vec3 OpB = A + AB * kB - O;
            vec3 OpC = A + AC * kC - O;

	    ivec2 b = ProjectCoord(OB, depthB);
	    ivec2 c = ProjectCoord(OC, depthC);
	    ivec2 opbn = ProjectCoord(OpB, nearplan);
	    ivec2 opcn = ProjectCoord(OpC, nearplan);

            vec2 VU = (V - U) * kB + U;
            vec2 WU = (W - U) * kC + U;
            vec3 nAB = (nB - nA) * kB + nA;
            vec3 nAC = (nC - nA) * kC + nA;

	    DrawTriangle(opbn, c, opcn,
                         (depthB - depthA) * kB + depthA,
                         depthC,
                         (depthC - depthA) * kC + depthA,
                         VU, W, WU,  nAB, nC, nAC);

	    DrawTriangle(opbn, b, c,
                         (depthB - depthA) * kB + depthA,
                         depthB, depthC,
                         VU, V, W,   nAB, nB, nC);
	} else if (depthB < nearplan) {
	    vec3 BC = C - B;
	    vec3 BA = A - B;
	    float kC = (nearplan - depthB) / dot(m_camera->j, BC);
	    float kA = (nearplan - depthB) / dot(m_camera->j, BA);

            vec3 OpC = B + BC * kC - O;
            vec3 OpA = B + BA * kA - O;
	    ivec2 c =  ProjectCoord(OC, depthC);
	    ivec2 a = ProjectCoord(OA, depthA);
	    ivec2 opcn = ProjectCoord(OpC, nearplan);
	    ivec2 opan = ProjectCoord(OpA, nearplan);

            vec2 VW = (W - V) * kC + V;
            vec2 VU = (W - U) * kA + V;
            vec3 nBC = (nC - nB) * kC + nB;
            vec3 nBA = (nA - nB) * kA + nB;

	    DrawTriangle(opcn, a, opan,
                         (depthC - depthB) * kC + depthB,
                         depthA,
                         (depthA - depthB) * kA + depthB,
                         VW, U, VU,   nBC, nA, nBA);
	    DrawTriangle(opcn, c, a,
                         (depthC - depthB) * kC + depthB,
                         depthC, depthA,
                         VW, W, U,  nBC, nC, nA);
	} else if (depthC < nearplan) {
	    vec3 CA = C - A;
	    vec3 CB = C - B;

	    float kA = (nearplan - depthC) / dot(m_camera->j, CA);
	    float kB = (nearplan - depthC) / dot(m_camera->j, CB);

            vec3 OpA = C + CA * kA - O;
            vec3 OpB = C + CB * kB - O;

	    ivec2 a = ProjectCoord(OA, depthA);
	    ivec2 b = ProjectCoord(OB, depthB);
	    ivec2 opan = ProjectCoord(OpA, nearplan);
	    ivec2 opbn = ProjectCoord(OpB, nearplan);

            vec2 WU = (U - W) * kA + W;
            vec2 WV = (V - W) * kB + W;
            vec3 nCA = (nA - nC) * kA + nC;
            vec3 nCB = (nB - nC) * kB + nC;
	    DrawTriangle(opan, b, opbn,
                         (depthA - depthC) * kA + depthC,
                         depthB,
                         (depthB - depthC) * kB + depthC,
                         WU, V, WV,  nCA, nB, nCB);

	    DrawTriangle(opan, a, b,
                         (depthA - depthC) * kA + depthC,
                         depthA, depthB,
                         WU, U, V,   nCA, nA, nB);
	}
    }
}

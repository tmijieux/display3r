#include <cmath>
#include <iostream>

#include "display3r/Color.hpp"
#include "display3r/Renderer.hpp"
#include "display3r/Window.hpp"
#include "display3r/Object3D.hpp"
#include "display3r/Object2D.hpp"

using namespace std;
using display3r::Renderer;
using display3r::Color;

// return the vector between camera.O and the intersection of
// (AB) and the NEARPLAN
vec3 Renderer::ProjectPoint(const vec3& A, const vec3& B)
{
    vec3 AB = B - A;
    vec3 d = A - m_lens->O;
    float k = (m_nearplan - dot(m_lens->j, d)) / dot(m_lens->j, AB);
    return A+AB*k - m_lens->O;
}

// return the Coord projection of a vector between scene.camera.O
// and A, knowing the depth of A
vec2 Renderer::ProjectCoord(vec3 const& OA, float depth)
{
    return vec2(
        m_width/(2.*tan(m_hfov / 2.)) *
        dot(m_lens->i, OA) / depth + m_width / 2,

        -m_height/(2.*tan(m_wfov / 2.)) *
        dot(m_lens->k, OA) / depth + m_height / 2);
}

void Renderer::DrawVertex(Vertex const &V)
{
    DrawVertex(V.position);
}

void Renderer::DrawVertex(vec3 const& A)
{
    vec3 OA = A - m_lens->O;
    float depthA = dot(m_lens->j, OA);
    vec2 p = ProjectCoord(OA, depthA);

    if (depthA > m_nearplan)
	DrawPixel(p, depthA);
}

void Renderer::DrawSegment(Segment const &S)
{
    DrawSegment(S.A, S.B);
}

void Renderer::DrawSegment(vec3 const &A, vec3 const &B)
{
    vec3 OA = A - m_lens->O;
    vec3 OB = B - m_lens->O;
    float depthA = dot(m_lens->j, OA);
    float depthB = dot(m_lens->j, OB);

    ivec2 t, u;
    if (depthA > m_nearplan &&  depthB > m_nearplan) {
	t = ProjectCoord(OA, depthA);
	u = ProjectCoord(OB, depthB);
    } else if (depthA < m_nearplan && depthB > m_nearplan) {
	vec3 AB = ProjectPoint(A, B);
	t = ProjectCoord(AB, m_nearplan);
	u = ProjectCoord(OB, depthB);
    } else if (depthA > m_nearplan && depthB < m_nearplan) {
	vec3 AB = ProjectPoint(A, B);
	t = ProjectCoord(OA, depthA);
	u = ProjectCoord(AB, m_nearplan);
    } else
	return;

    DrawLine(t, u, depthA, depthB);
}


void Renderer::DrawFace(Face const &f)
{
    DrawFace(f.A.position, f.B.position, f.C.position,
             f.A.texcoord, f.B.texcoord, f.C.texcoord,
             f.A.normal, f.B.normal, f.C.normal);
}

void Renderer::DrawFace(vec3 const &A, vec3 const &B, vec3 const &C,
                        vec2 const &U, vec2 const &V, vec2 const &W,
                        vec3 const &nA, vec3 const &nB, vec3 const &nC)
{
    vec3 O = m_lens->O;
    vec3 OA = A - O;
    vec3 OB = B - O;
    vec3 OC = C - O;
    float dA = dot(m_lens->j, OA);
    float dB = dot(m_lens->j, OB);
    float dC = dot(m_lens->j, OC);

    if (dA > m_nearplan && dB > m_nearplan && dC > m_nearplan) {
	ivec2 a = ProjectCoord(OA, dA);
	ivec2 b = ProjectCoord(OB, dB);
	ivec2 c = ProjectCoord(OC, dC);
        Color lA = ComputeLight(A, nA);
        Color lB = ComputeLight(B, nB);
        Color lC = ComputeLight(C, nC);

	DrawTriangle(Pixel(a, dA, lA, U),
                     Pixel(b, dB, lB, V),
                     Pixel(c, dB, lC, W));

    } else if (dB > m_nearplan && dC > m_nearplan)
        DrawTwoSubFaces(A, B, C, dA, dB, dC, OB, OC, U, V, W, nA, nB, nC);
    else if (dC > m_nearplan && dA > m_nearplan)
        DrawTwoSubFaces(B, C, A, dB, dC, dA, OC, OA, V, W, U, nB, nC, nA);
    else if (dA > m_nearplan && dB > m_nearplan)
        DrawTwoSubFaces(C, A, B, dC, dA, dB, OA, OB, W, U, V, nC, nA, nB);
    else if (dA > m_nearplan)
        DrawOneSubFace(A, B, C, dA, dB, dC, OA, U, V, W, nA, nB, nC);
    else if (dB > m_nearplan)
        DrawOneSubFace(B, C, A, dB, dC, dA, OB, V, W, U, nB, nC, nA);
    else if (dC > m_nearplan)
        DrawOneSubFace(C, A, B, dC, dA, dB, OC, W, U, V, nC, nA, nB);
}


void Renderer::DrawTwoSubFaces(
    vec3 const &A, vec3 const &B, vec3 const &C,
    float dA, float dB, float dC,
    vec3 const &OB, vec3 const &OC,
    vec2 const &U, vec2 const &V,  vec2 const &W,
    vec3 const &nA, vec3 const &nB, vec3 const &nC)
{
    vec3 AB = B - A;
    vec3 AC = C - A;
    float kB = (m_nearplan - dA) / dot(m_lens->j, AB);
    float kC = (m_nearplan - dA) / dot(m_lens->j, AC);

    vec3 OpB = A + AB * kB - m_lens->O;
    vec3 OpC = A + AC * kC - m_lens->O;

    ivec2 b = ProjectCoord(OB, dB);
    ivec2 c = ProjectCoord(OC, dC);
    ivec2 opbn = ProjectCoord(OpB, m_nearplan);
    ivec2 opcn = ProjectCoord(OpC, m_nearplan);

    vec2 VU = (V - U) * kB + U;
    vec2 WU = (W - U) * kC + U;

    vec3 nAB = (nB - nA) * kB + nA;
    vec3 nAC = (nC - nA) * kC + nA;

    Color lB = ComputeLight(B, nB);
    Color lC = ComputeLight(C, nC);
    Color lBn = ComputeLight(OpB, nAB);
    Color lCn = ComputeLight(OpC, nAC);

    Pixel pCn(opcn, (dC - dA)*kC + dA, lCn, WU);
    Pixel pBn(opbn, (dB - dA)*kB + dA, lBn, VU);
    Pixel pC(c, dC, lC, W);
    Pixel pB(b, dB, lB, V);
    DrawTriangle(pBn, pC, pCn);
    DrawTriangle(pBn, pB, pC);
}


void Renderer::DrawOneSubFace(
    vec3 const &A, vec3 const &B, vec3 const &C,
    float dA, float dB, float dC,
    vec3 const &OA,
    vec2 const &U, vec2 const &V, vec2 const &W,
    vec3 const &nA, vec3 const &nB, vec3 const &nC)
{
    vec3 AB = B - A;
    vec3 AC = C - A;
    float kB = (m_nearplan - dA) / dot(m_lens->j, AB);
    float kC = (m_nearplan - dA) / dot(m_lens->j, AC);

    vec3 OpB = A + AB * kB - m_lens->O;
    vec3 OpC = A + AC * kC - m_lens->O;

    ivec2 a = ProjectCoord(OA, dA);
    ivec2 bn = ProjectCoord(OpB, m_nearplan);
    ivec2 cn = ProjectCoord(OpC, m_nearplan);

    vec2 UV = (V - U) * kB + U;
    vec2 UW = (W - U) * kC + U;

    vec3 nAB = (nB - nA) * kB + nA;
    vec3 nAC = (nC - nA) * kC + nA;

    Color lA = ComputeLight(A, nA);
    Color lB = ComputeLight(OpB, nAB);
    Color lC = ComputeLight(OpC, nAC);

    DrawTriangle(Pixel(a, dA, lA, U),
                 Pixel(bn, (dB - dA)*kB+dA, lB, UV),
                 Pixel(cn, (dC - dA)*kC+dA, lC, UW));
}

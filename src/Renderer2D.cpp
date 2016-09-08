#include <SDL.h>
#include <math.h>

#include "display3r/Color.hpp"
#include "display3r/Renderer.hpp"
#include "display3r/Window.hpp"

using display3r::Renderer;
using display3r::Color;
using display3r::Pixel;
using display3r::Line;
using display3r::Triangle;

namespace display3r {

static inline int triple(ivec2 a, ivec2 b)
{
    return a.x*b.y-a.y*b.x;
}

static inline void TexCoordLoop(vec2 &a)
{
    
}

};

void Renderer::DrawPixel(Pixel const &P)
{
    DrawPixelInternal(P, m_drawColor);
}

void Renderer::DrawPixelInternal(Pixel const &P, Color c)
{
    if (m_window->IsValidCoordinate(P.A) &&
        (m_zbuf[P.A] < 0 || m_zbuf[P.A] > P.depth))
    {
	m_window->SetPixel(P.A, c);
        m_zbuf[P.A] = P.depth;
    }
}

void Renderer::DrawLine(Line const &L)
{
    DrawLine(L.A, L.B, L.dA, L.dB);
}

void Renderer::DrawLine(ivec2 const &A, ivec2 const &B, float dA, float dB)
{
    ivec2 M = A;
    int dx = B.x - A.x;
    int dy = B.y - A.y;
    
    int xIncr = dx > 0 ? 1 : -1;
    int yIncr = dy > 0 ? 1 : -1;
    
    float alpha, depthM;
    int error;
    
    if ((abs(dx) > abs(dy))) {
	error = dx;
	dx = 2 * dx;
	dy = 2 * dy;
	while (M.x != B.x) {
	    M.x += xIncr;
	    alpha = (float) (M.x - A.x) / (B.x - A.x);
	    depthM = dA * dB / ((1 - alpha) * dB + alpha * dA);
	    if ((error -= yIncr * dy) <= 0) {
		M.y += yIncr;
		error += xIncr * dx;
	    }
            DrawPixel(Pixel(M, depthM));
	}
    } else {
	error = dy;
	dy = 2 * dy;
	dx = 2 * dx;
	while (M.y != B.y) {
	    M.y += yIncr;
	    alpha = (float) (M.y - A.y) / (B.y - A.y);
	    depthM = dA * dB / ((1 - alpha) * dB + alpha * dA);
	    if ((error -= xIncr * dx) <= 0) {
		M.x += xIncr;
		error += yIncr * dy;
	    }
            DrawPixel(Pixel(M, depthM));
	}
    }
}

void Renderer::DrawTriangle(Triangle const &T)
{
    DrawTriangle(T.A, T.B, T.C, T.dA, T.dB, T.dC,
                 T.U, T.V, T.W, T.nA, T.nB, T.nC);
}

void Renderer::DrawTriangle(ivec2 const &A, ivec2 const &B, ivec2 const &C,
                            float dA, float dB, float dC,
                            vec2 const &U, vec2 const &V, vec2 const &W,
                            vec3 const &nA, vec3 const &nB, vec3 const &nC)
{
    
    
    ivec2 AB = B - A;
    ivec2 BC = C - B;

    if (triple(AB, BC) <= 0)
	return;
    
    ivec2 WH(m_window->GetWidth(),m_window->GetHeight());
    // ivec2 minABC = max(min(min(A, B), C), WH);
    // ivec2 maxABC = min(max(max(A, B), C), WH);
    
    int minW = max(min(min(A.x, B.x), C.x), 0);
    int maxW = min(max(max(A.x, B.x), C.x), WH.x);
    int minH = max(min(min(A.y, B.y), C.y), 0);
    int maxH = min(max(max(A.y, B.y), C.y), WH.y);

    ivec2 AC = C - A;

    vec3 p = nA + m_light;
    float scaleA = 1 - glm::length(p) / 2;
    p = nB + m_light;
    float scaleB = 1 - glm::length(p) / 2;
    p = nC + m_light;
    float scaleC = 1 - glm::length(p) / 2;

    float scaleABC = scaleA * scaleB * scaleC;
    
    float scaleAB = scaleA * scaleB;
    float scaleBC = scaleB * scaleC;
    float scaleCA = scaleC * scaleA;

    float dABC = dA * dB * dC;

    float dAB = dA * dB;
    float dBC = dB * dC;
    float dCA = dC * dA;

    float scale;
    float depthM;

    vec2 N, u, v, w;
    if (m_texture != NULL) {
        u = U * (1.f / dA);
        v = V * (1.f / dB);
        w = W * (1.f / dC);
    }

    int det = triple(AB, AC);

    ivec2 M;
    ivec2 AM, BM, CM;
    for (M.y = minH; M.y <= maxH; ++M.y) {
	M.x = minW;
	AM = M - A;
	BM = M - B;
	CM = M - C;
	while (M.x <= maxW &&
               (triple(AB, AM) < 0 || triple(BC, BM) < 0 || triple(CM, AC) < 0))
        {
	    M.x++;
	    AM = M - A;
	    BM = M - B;
	    CM = M - C;
	}
	while (M.x <= maxW &&
               triple(AB, AM) >= 0 && triple(BC, BM) >= 0 && triple(CM, AC) >= 0)
        {
	    float alpha = (float) triple(AB, AM) / det;
	    float beta = (float) triple(AM, AC) / det;
	    float gamma = 1. - alpha - beta;

	    depthM = dABC / 
		(gamma * dBC + beta * dCA + alpha * dAB);


            Color c;
	    if (m_zbuf[M] < 0 || m_zbuf[M] > depthM) {
		scale = scaleABC / 
		    (gamma * scaleBC + beta * scaleCA + alpha * scaleAB);
		
		if (m_texture != NULL) {
		    N.x = (gamma * u.x + beta * v.x + alpha * w.x) / 
			(gamma / dA + beta / dB + alpha / dC);
		    N.y = (gamma * u.y + beta * v.y + alpha * w.y) /
			(gamma / dA + beta / dB + alpha / dC);
		    TexCoordLoop(N);
                    c = m_texture->GetColor(N);
		} else
                    c = Color(255, 0, 255);
		DrawPixelInternal(Pixel(M, depthM), c*scale);
	    }
	    M.x++;
	    AM = M - A;
	    BM = M - B;
	    CM = M - C;
	}
    }
}

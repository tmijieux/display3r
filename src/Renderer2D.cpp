#include <SDL.h>
#include <math.h>

#include "display3r/Color.hpp"
#include "display3r/Renderer.hpp"
#include "display3r/Window.hpp"
#include "display3r/Object2D.hpp"

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

static inline void loopFLoat(float &f)
{
    if (f < 0.)
        f =  f - (int) f + 1;
    else if (f >= 1)
        f = f - (int) f;
}

static inline void TexCoordLoop(vec2 &a)
{
    loopFLoat(a.x);
    loopFLoat(a.y);
}

};

void Renderer::DrawLexel(ivec2 P, Color color)
{
    Color filter = m_lens->GetFilter();
    P += m_lens->GetWindowPosition();
    color.Filter(filter);

    if (m_lens->Overlapping()) {
        Color back = m_window->GetPixel(P);
        color.Average(back, 128);
    }
    m_window->SetPixel(P, color);
}

void Renderer::DrawPixel(ivec2 const &P, float depth)
{
    if (m_window->IsValidCoordinate(P) &&
        (m_zbuf[P] < m_nearplan || m_zbuf[P] > depth))
    {
        DrawLexel(P, m_drawColor);
        m_zbuf[P] = depth;
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

    float alpha = (float) (M.x - A.x) / (B.x - A.x);
    float depthM = dA * dB / ((1 - alpha) * dB + alpha * dA);

    DrawPixel(M, depthM);

    if ((abs(dx) > abs(dy))) {
	int error = dx;
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
            DrawPixel(M, depthM);
	}
    } else {
	int error = dy;
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
            DrawPixel(M, depthM);
	}
    }
}

void Renderer::DrawTriangle(Triangle const &T)
{
    DrawTriangle(Pixel(T.A, T.dA, T.lA, T.U),
                 Pixel(T.B, T.dB, T.lB, T.V),
                 Pixel(T.C, T.dC, T.lC, T.W));
}

void Renderer::DrawTriangle(Pixel const &A, Pixel const &B, Pixel const &C)
{
    ivec2 AB = B.pos - A.pos;
    ivec2 BC = C.pos - B.pos;
    ivec2 CA = A.pos - C.pos;

    if (triple(AB, BC) <= 0)
	return;

    int minW = MAX(0, MIN(MIN(A.pos.x, B.pos.x), C.pos.x));
    int maxW = MIN(m_width - 1, MAX(MAX(A.pos.x, B.pos.x), C.pos.x));

    int minH = MAX(0, MIN(MIN(A.pos.y, B.pos.y), C.pos.y));
    int maxH = MIN(m_height - 1, MAX(MAX(A.pos.y, B.pos.y), C.pos.y));

    float dABC = A.depth * B.depth * C.depth;

    float dAB = A.depth * B.depth;
    float dBC = B.depth * C.depth;
    float dCA = C.depth * A.depth;

    vec2 u, v, w;
    if (m_texture) {
        u = A.texcoord * (1.f / A.depth);
        v = B.texcoord * (1.f / B.depth);
        w = C.texcoord * (1.f / C.depth);
    }

    int det = triple(CA, AB);
    ivec2 M;

    for (M.y = minH; M.y <= maxH; ++M.y) {
	ivec2 AM, BM, CM;
	int PAlpha, PBeta, PGamma;
	M.x = minW;

	do {
            AM = M - A.pos;
            BM = M - B.pos;
            CM = M - C.pos;
	    M.x++;
	} while (M.x <= maxW &&
                 (triple(AB, AM) < 0 ||
                  triple(BC, BM) < 0 ||
                  triple(CA, CM) < 0));
	M.x--;
	while (M.x <= maxW &&
	       (PAlpha = triple(BC, BM)) >= 0 &&
	       (PBeta = triple(CA, CM)) >= 0 &&
	       (PGamma = triple(AB, AM)) >= 0)
        {
	    float alpha = (float) PAlpha / (float) det;
	    float beta = (float) PBeta / (float) det;
	    float gamma = (float) PGamma / (float) det;

	    float depthM = dABC /
                (alpha * dBC + beta * dCA + gamma * dAB);

	    if (m_zbuf[M] < m_nearplan || m_zbuf[M] > depthM)
            {
		Color c, colorM;
                colorM = Interpolate(A.light, B.light, C.light, alpha, beta, gamma);

		if (m_texture) {
		    float d;
                    d = (alpha / A.depth + beta / B.depth + gamma / C.depth);
		    vec2 N = (u * alpha + v * beta + w * gamma) * (1.f / d);
		    TexCoordLoop(N);
		    c = m_texture->GetColor(N);
		} else
		    c = m_untexturedColor;
                c = colorM * c;
		DrawLexel(M, c);
		m_zbuf[M] = depthM;
	    }
	    M.x++;
            AM = M - A.pos;
            BM = M - B.pos;
            CM = M - C.pos;
	}
    }
}

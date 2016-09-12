#include <string>
#include <ncurses.h>

#include "display3r/Window.hpp"
#include "display3r/Exception.hpp"
#include "display3r/Util.hpp"
#include "display3r/PluginDetails.hpp"
#include "display3r/Camera.hpp" // for directions
#include "display3r/ncurses/backend_ncurses.hpp"

#define RANGENC 1000
#define DOWNER 64
#define UPPER 192

static inline int powi(int a, int b)
{
    int p = 1;
    for (int i = 0; i < b; i++)
	p *= a;
    return p;
}

static int GetRange(int nbColor)
{
    int a = 0;
    int b = nbColor;
    while (b - a > 1) {
	int c = (a + b) / 2;
	if (powi(c, 3) > nbColor)
	    b = c;
	else
	    a = c;
    }
    return a;
}

// 0 <= COLOR <= 1000 et 0 <= r, g et b < _range1
void backend_ncurses::GetCOLORFromRGB(int *r, int *g, int *b)
{
    *r = *r * RANGENC / (_range1 - 1);
    *g = *g * RANGENC / (_range1 - 1);
    *b = *b * RANGENC / (_range1 - 1);
}

// 0 <= id < nbColor et 0 <= r, g et b < _range1
void backend_ncurses::GetRGBFromID(int id, int *r, int *g, int *b)
{
    *r = id / _range2;
    id -= *r * _range2;
    *g = id / _range1;
    id -= *g * _range1;
    *b = id;
}

void backend_ncurses::GetRGBFromColor(Color c, int *r, int *g, int *b)
{
    *r = c.r * (_range1 - 1) / 255;
    *g = c.g * (_range1 - 1) / 255;
    *b = c.b * (_range1 - 1) / 255;
}

int backend_ncurses::GetIDFromRGB(int r, int g, int b)
{
    int id = r * _range2 + g * _range1 + b;
    if (id == 0)
	return _background;
    else if (id == _background)
	return 0;
    else
	return id;
}

// 0 <= id < nbColor et 0 <= r, g et b < _range1
int backend_ncurses::GetIDFromColor(Color c)
{
    int r, g, b;
    GetRGBFromColor(c, &r, &g, &b);
    return GetIDFromRGB(r, g, b);
}

void backend_ncurses::GetCOLORFromID(int id, int *r, int *g, int *b)
{
    GetRGBFromID(id, r, g, b);
    GetCOLORFromRGB(r, g, b);
}

void backend_ncurses::SetIDWithCOLOR(int id, int r, int g, int b)
{
    init_color(id, r, g, b);
    init_pair(id, id, id);
}

void backend_ncurses::GetIntervalComponent(
    int coord, int component, int *downer, int * upper)
{
    if (coord < DOWNER) {
	*downer = component;
	*upper = component;
    } else if (coord >= UPPER) {
	*downer = component + 1;
	*upper = component + 1;
    } else {
	*downer = component;
	*upper = component + 1;
    }
}

void backend_ncurses::GetIntervalID(Color c, int *downer, int *upper)
{
    int r, g, b;
    GetRGBFromColor(c, &r, &g, &b);
    int R = c.r * (_range1 - 1) - r * 255;
    int G = c.g * (_range1 - 1) - g * 255;
    int B = c.b * (_range1 - 1) - b * 255;
    int downerR, upperR, downerG, upperG, downerB, upperB;
    GetIntervalComponent(R, r, &downerR, &upperR);
    GetIntervalComponent(G, g, &downerG, &upperG);
    GetIntervalComponent(B, b, &downerB, &upperB);
    *downer = GetIDFromRGB(downerR, downerG, downerB);
    *upper = GetIDFromRGB(upperR, upperG, upperB);
}

bool backend_ncurses::InitColor(Color background)
{
    if (can_change_color() == FALSE)
	return false;
    int r, g, b;
    start_color();
    _range1 = GetRange(MIN(COLOR_PAIRS, COLORS));
    _range2 = SQUARE(_range1);
    _nbColor = _range2 * _range1;

    for (int i = 1; i < _nbColor; i++) {
	GetCOLORFromID(i, &r, &g, &b);
	SetIDWithCOLOR(i, r, g, b);
    }
    _background = GetIDFromColor(background);
    GetCOLORFromID(_background, &r, &g, &b);
    SetIDWithCOLOR(_background, 0, 0, 0);
    SetIDWithCOLOR(0, r, g, b);
    return true;
}

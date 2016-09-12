#ifndef DISPLAY3R_NCURSES_WINDOW_H
#define DISPLAY3R_NCURSES_WINDOW_H

#include "display3r/Window.hpp"
#include "display3r/WindowMaker.hpp"

class backend_ncurses : public display3r::Window {
    typedef display3r::Event Event;
    typedef display3r::Color Color;
public:
    backend_ncurses(int width, int height, Color c);
    ~backend_ncurses() override;

    int GetWidth() override;
    int GetHeight() override;
    void Clear() override;
    void Update() override;
    bool PollEvent(Event&) override;
    void WaitEvent(Event&) override;
    void SetPixel(ivec2 const &coord, Color const &color) override;
    Color GetPixel(ivec2 const &coord) override;

private:
    void ParseEvent(Event &event, int c);

    void GetCOLORFromRGB(int *r, int *g, int *b);
    void GetCOLORFromID(int id, int *r, int *g, int *b);
    void GetRGBFromID(int id, int *r, int *g, int *b);
    void GetRGBFromColor(Color c, int *r, int *g, int *b);
    int GetIDFromRGB(int r, int g, int b);
    int GetIDFromColor(Color c);
    void SetIDWithCOLOR(int id, int r, int g, int b);
    void GetIntervalComponent(int coord, int component,
                              int *downer, int * upper);
    void GetIntervalID(Color c, int *downer, int *upper);
    bool InitColor(Color color);

private:
    int _background;
    short _nbColor;
    short _range1;
    short _range2;
};

class backend_ncurses_factory : public display3r::IWindowMaker
{
public:
    display3r::Window *MakeWindow(
        int width, int height, display3r::Color c) override;

private:
    friend class backend_ncurses;
    static backend_ncurses *sWindowInstance;
    void InitNCurses();
};

#endif // DISPLAY3R_NCURSES_WINDOW_H

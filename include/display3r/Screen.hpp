#ifndef SCREEN_H
#define SCREEN_H

struct Screen {
    int width;
    int height;

    Screen(int a, int b): width(a), height(b) {}
private:
    SDL_Surface *screen;
};

#endif //SCREEN_H

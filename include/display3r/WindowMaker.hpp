#ifndef WINDOWMAKER_H
#define WINDOWMAKER_H

#include <vector>
#include <memory>
#include "display3r/Color.hpp"

namespace display3r {
class SharedLibrary;
class Config;
class Window;

class IWindowMaker {
public:
    virtual Window *MakeWindow(int width, int height, Color c) = 0;
};

class WindowMaker {
public:
    Window *MakeWindow(Config const &conf);

private:
    std::vector<std::shared_ptr<SharedLibrary> > m_dlls;
};

};
#endif //WINDOWMAKER_H

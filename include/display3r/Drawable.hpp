#ifndef DISPLAY3R_DRAWABLE_H
#define DISPLAY3R_DRAWABLE_H

namespace display3r {

class Renderer;

class Drawable {
public:
    Drawable();
    void Draw(Renderer &renderer);
    void SetHidden(bool value);
    void ToggleVisibility();

protected:
    virtual void DrawHandler(Renderer &renderer) = 0;

private:
    bool m_hidden;
};

};
#endif //DISPLAY3R_DRAWABLE_H

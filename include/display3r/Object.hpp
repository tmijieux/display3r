#ifndef OBJECT_H
#define OBJECT_H

namespace display3r {

class Renderer;

class Object {
public:
    Object();
    void Draw(Renderer &renderer);
    void SetHidden(bool value);
    void ToggleVisibility();

protected:
    virtual void DrawHandler(Renderer &renderer) = 0;

private:
    bool m_hidden;
};

};
#endif //OBJECT_H

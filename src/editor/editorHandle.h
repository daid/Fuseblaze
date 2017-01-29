#ifndef EDITOR_EDITOR_HANDLE_H
#define EDITOR_EDITOR_HANDLE_H

#include <sp2/scene/node.h>

class EditorHandle : public sp::SceneNode
{
public:
    enum Type
    {
        Rotation,
        ScaleX,
        ScaleY
    };

    EditorHandle(sp::P<sp::SceneNode> parent, sp::Vector2d offset, Type type);
    virtual void dragTo(sp::Vector2d position);

private:
    double rotation_offset;
    Type type;
};

#endif//EDITOR_EDITOR_HANDLE_H

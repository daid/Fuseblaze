#ifndef FLOOR_H
#define FLOOR_H

#include <sp2/scene/node.h>
#include <sp2/graphics/meshdata.h>

class Floor : public sp::SceneNode
{
public:
    Floor(sp::Vector2d position, double rotation, sp::Vector2f size, sp::Color color)
    : sp::SceneNode(::scene->getRoot())
    {
        render_data.type = sp::RenderData::Type::Normal;
        render_data.shader = sp::Shader::get("shader/color.shader");
        render_data.mesh = sp::MeshData::createQuad(size);
        render_data.color = color;
        render_data.order = -10;//Draw floors at bottom most layer, below anything else.
        setPosition(position);
        setRotation(rotation);
    }
};

#endif//FLOOR_H

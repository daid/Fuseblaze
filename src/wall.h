#ifndef WALL_H
#define WALL_H

#include "main.h"
#include "shadowCastNode.h"
#include <sp2/collision/2d/box.h>

class Wall : public sp::Node
{
public:
    Wall(sp::Vector2d position, double rotation, sp::Vector2f size, sp::Color color)
    : sp::Node(::scene->getRoot())
    {
        render_data.type = sp::RenderData::Type::Normal;
        render_data.shader = sp::Shader::get("shader/color.shader");
        render_data.mesh = sp::MeshData::createQuad(size);
        render_data.color = color;
        render_data.order = 5;//Draw walls on top of basic objects, but below effects
        setPosition(position);
        setRotation(rotation);

        sp::collision::Box2D box(size.x, size.y);
        box.type = sp::collision::Shape::Type::Static;
        setCollisionShape(box);
        
        new ShadowCastNode(this, size);
    }
};

#endif//WALL_H

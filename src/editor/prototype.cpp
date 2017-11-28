#include "prototype.h"
#include "editorHandle.h"
#include "../main.h"

#include <sp2/graphics/meshdata.h>
#include <sp2/collision/2d/box.h>
#include <sp2/random.h>

Prototype::Prototype(Prefab::Part::Type type, sp::Vector2d position, double rotation, sp::Vector2f size)
: sp::Node(::scene->getRoot()), type(type)
{
    render_data.type = sp::RenderData::Type::Normal;
    render_data.shader = sp::Shader::get("shader/color.shader");
    setPosition(position);
    setRotation(rotation);

    switch(type)
    {
    case Prefab::Part::Type::Floor:
        render_data.order = -10;
        color = sp::HsvColor(sp::random(100, 130), 50, 20);
        break;
    case Prefab::Part::Type::Wall:
        render_data.order = 5;
        color = sp::HsvColor(sp::random(100, 130), 50, 50);
        break;
    case Prefab::Part::Type::Door:
        render_data.order = 4;
        color = sp::HsvColor(sp::random(100, 130), 10, 50);
        break;
    case Prefab::Part::Type::Trigger:
        render_data.order = -1;
        color = sp::HsvColor(sp::random(0, 360), 100, 100);
        break;
    case Prefab::Part::Type::PrefabConnection:
        render_data.order = -2;
        color = sp::HsvColor(0, 0, 50);
        break;
    case Prefab::Part::Type::Spawner:
        render_data.order = -1;
        color = sp::HsvColor(0, 100, 50);
        break;
    }

    setColor(color);
    setSize(size);
}

void Prototype::setSize(sp::Vector2f size)
{
    render_data.mesh = sp::MeshData::createQuad(size);

    sp::collision::Box2D box(size.x, size.y);
    box.type = sp::collision::Shape::Type::Sensor;
    setCollisionShape(box);
    
    this->size = size;
}

void Prototype::setColor(sp::HsvColor color)
{
    this->color = color;
    render_data.color = color;
}

void Prototype::createHandles()
{
    new EditorHandle(this, sp::Vector2d(size) / 2.0 + sp::Vector2d(0.5, 0.5), EditorHandle::Type::Rotation);
    new EditorHandle(this, sp::Vector2d(-size.x, 0.0) / 2.0 - sp::Vector2d(0.5, 0.0), EditorHandle::Type::ScaleX);
    new EditorHandle(this, sp::Vector2d(0.0, -size.y) / 2.0 - sp::Vector2d(0.0, 0.5), EditorHandle::Type::ScaleY);
}

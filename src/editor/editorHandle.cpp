#include "editorHandle.h"
#include "prototype.h"

#include <sp2/graphics/meshdata.h>

EditorHandle::EditorHandle(sp::P<sp::SceneNode> parent, sp::Vector2d offset, Type type)
: sp::SceneNode(parent), type(type)
{
    setPosition(offset);

    render_data.type = sp::RenderData::Type::Normal;
    render_data.shader = sp::Shader::get("shader/color.shader");
    render_data.color = sf::Color::Blue;
    render_data.order = 100;

    render_data.mesh = sp::MeshData::createQuad(sp::Vector2f(1, 1));
    
    rotation_offset = sp::toRotationAngle(offset);
}

void EditorHandle::dragTo(sp::Vector2d position)
{
    sp::P<Prototype> prototype = getParent();

    sp::Vector2d point = prototype->getGlobalTransform().inverse() * position;
    double step_size = 1.0;
    if (prototype->type == Prefab::Part::Type::PrefabConnection)
        step_size = 3.0;
    point = sp::Vector2d(sf::Vector2i(sp::abs(point * 2.0) / step_size + sp::Vector2d(0.5, 0.5))) * step_size - sp::Vector2d(1.0, 1.0);
    point.x = std::max(point.x, 1.0);
    point.y = std::max(point.y, 1.0);
    switch(type)
    {
    case Type::Rotation:
        {
            double angle = sp::toRotationAngle(position - prototype->getGlobalPosition2D()) - rotation_offset;
            angle = int(angle / 15) * 15;
            prototype->setRotation(angle);
        }
        break;
    case Type::ScaleX:
        prototype->setSize(sp::Vector2f(point.x, prototype->size.y));
        break;
    case Type::ScaleY:
        prototype->setSize(sp::Vector2f(prototype->size.x, point.y));
        break;
    }
}

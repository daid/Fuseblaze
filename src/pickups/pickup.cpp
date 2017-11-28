#include "pickup.h"
#include "../main.h"
#include "../player.h"

#include <sp2/graphics/meshdata.h>
#include <sp2/collision/2d/circle.h>

Pickup::Pickup()
: sp::Node(::scene->getRoot())
{
    render_data.type = sp::RenderData::Type::Normal;
    render_data.shader = sp::Shader::get("shader/color.shader");
    render_data.mesh = sp::MeshData::createQuad(sp::Vector2f(0.3, 0.3));
    render_data.order = -2;
    render_data.color = sf::Color::White;

    sp::collision::Circle2D circle(0.5);
    circle.type = sp::collision::Shape::Type::Sensor;
    setCollisionShape(circle);
    
    name = "?";
}

void Pickup::pickUpBy(sp::P<Player> player)
{
    delete this;
}

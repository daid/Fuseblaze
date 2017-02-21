#include "hulk.h"
#include "../shadowCastNode.h"

#include <sp2/collision/2d/circle.h>

Hulk::Hulk()
{
    vision_range = 35.0;
    health = 50.0;
    movement_speed = 7.0;

    render_data.type = sp::RenderData::Type::Normal;
    render_data.shader = sp::Shader::get("shader/color.shader");
    render_data.mesh = sp::MeshData::createQuad(sp::Vector2f(3, 3));
    render_data.color = sp::HsvColor(120, 70, 50);
    
    sp::collision::Circle2D circle(1.5);
    circle.angular_damping = 5;
    circle.linear_damping = 5;
    setCollisionShape(circle);
    
    new ShadowCastNode(this, sp::Vector2f(2, 2));
}

void Hulk::takeDamage(double amount)
{
    render_data.color = sp::HsvColor(health / 50.0 * 120, 70, 50);
    //takeDamage can delete this object, so we cannot access members after calling it.
    Enemy::takeDamage(amount);
}

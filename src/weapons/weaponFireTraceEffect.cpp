#include "weaponFireTraceEffect.h"
#include "../main.h"
#include <sp2/tween.h>
#include <sp2/graphics/spriteManager.h>

WeaponFireTraceEffect::WeaponFireTraceEffect(sp::Vector2d start_position, sp::Vector2d end_position)
: sp::Node(::scene->getRoot())
{
    setPosition((start_position + end_position) / 2.0);
    setRotation(sp::toRotationAngle(end_position - start_position));
    
    render_data = sp::SpriteManager::get("weapon_trace");
    render_data.type = sp::RenderData::Type::Additive;
    render_data.order = 20; //render effects on top of everything else
    render_data.scale = sp::Vector3f(sp::length(end_position - start_position), 1.0, 1.0);
    
    lifetime = 0.2;
}

void WeaponFireTraceEffect::onUpdate(float delta)
{
    lifetime -= delta;
    
    render_data.color = sp::Tween<sp::Color>::easeOutQuad(lifetime, 0.2, 0.0, sp::Color(255, 255, 255, 192), sp::Color(255, 255, 255, 0));
    
    if (lifetime < 0.0)
        delete this;
}

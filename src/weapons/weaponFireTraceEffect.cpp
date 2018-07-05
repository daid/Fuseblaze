#include "weaponFireTraceEffect.h"
#include "../main.h"
#include <sp2/tween.h>
#include <sp2/graphics/meshData.h>
#include <sp2/graphics/textureManager.h>

WeaponFireTraceEffect::WeaponFireTraceEffect(sp::Vector2d start_position, sp::Vector2d end_position)
: sp::Node(::scene->getRoot())
{
    setPosition((start_position + end_position) / 2.0);
    setRotation((end_position - start_position).angle());
    
    render_data.mesh = sp::MeshData::createQuad(sp::Vector2f(1, 0.1));
    render_data.texture = sp::texture_manager.get("weapons/trace.png");
    render_data.shader = sp::Shader::get("internal:basic.shader");
    render_data.type = sp::RenderData::Type::Additive;
    render_data.order = 20; //render effects on top of everything else
    render_data.scale = sp::Vector3f((end_position - start_position).length(), 1.0, 1.0);
    
    lifetime = 0.2;
}

void WeaponFireTraceEffect::onUpdate(float delta)
{
    lifetime -= delta;
    
    render_data.color = sp::Tween<sp::Color>::easeOutQuad(lifetime, 0.2, 0.0, sp::Color(1, 1, 1, 0.75), sp::Color(1, 1, 1, 0));
    
    if (lifetime < 0.0)
        delete this;
}

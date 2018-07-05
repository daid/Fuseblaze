#include "blood.h"
#include "main.h"
#include <sp2/tween.h>
#include <sp2/random.h>
#include <sp2/graphics/meshdata.h>
#include <sp2/graphics/textureManager.h>

BloodDecal::BloodDecal(sp::Vector2d position)
: sp::Node(::scene->getRoot())
{
    setPosition(position);
    setRotation(sp::random(0, 360));
    
    render_data.mesh = sp::MeshData::createQuad(sp::Vector2f(1.0, 1.0));
    render_data.texture = sp::texture_manager.get("effect/blood.png");
    render_data.shader = sp::Shader::get("internal:basic.shader");
    render_data.type = sp::RenderData::Type::Additive;
    render_data.order = -1; //render blood below walls, enemies and players, but above floors
    
    lifetime = 60.0;
}

void BloodDecal::onUpdate(float delta)
{
    lifetime -= delta;
    
    render_data.color = sp::Tween<sp::Color>::easeOutQuad(lifetime, 60.0, 0.0, sp::Color(1,0,0), sp::Color(1, 0, 0, 0));
    render_data.scale = sp::Tween<sp::Vector3f>::easeOutQuad(lifetime, 60.0, 0.0, sp::Vector3f(1.0, 1.0, 1.0), sp::Vector3f(0.1, 0.1, 1.0));
    
    if (lifetime < 0.0)
        delete this;
}

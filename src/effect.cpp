#include "effect.h"
#include "main.h"
#include <sp2/tween.h>
#include <sp2/graphics/meshData.h>
#include <sp2/graphics/textureManager.h>

Effect::Effect(sp::Vector2d position, float scale)
: sp::Node(::scene->getRoot()), scale(scale)
{
    setPosition(position);
    
    render_data.mesh = sp::MeshData::createQuad(sp::Vector2f(1, 1));
    render_data.texture = sp::texture_manager.get("effect/ring.png");
    render_data.shader = sp::Shader::get("internal:basic.shader");
    render_data.type = sp::RenderData::Type::Additive;
    render_data.order = 20; //render effects on top of everything else
    
    lifetime = 0.3;
}

void Effect::onUpdate(float delta)
{
    lifetime -= delta;
    
    render_data.color = sp::Tween<sp::Color>::easeOutQuad(lifetime, 0.3, 0.0, sp::Color(1,1,1), sp::Color(1, 1, 1, 0));
    render_data.scale = sp::Tween<sp::Vector3f>::easeOutQuad(lifetime, 0.3, 0.0, sp::Vector3f(scale * 0.1, scale * 0.1, 1.0), sp::Vector3f(scale * 2.0, scale * 2.0, 1.0));
    
    if (lifetime < 0.0)
        delete this;
}

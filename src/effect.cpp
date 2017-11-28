#include "effect.h"
#include "main.h"
#include <sp2/tween.h>
#include <sp2/graphics/spriteManager.h>

Effect::Effect(sp::Vector2d position, float scale)
: sp::Node(::scene->getRoot()), scale(scale)
{
    setPosition(position);
    
    render_data = sp::SpriteManager::get("effect_ring");
    render_data.type = sp::RenderData::Type::Additive;
    render_data.order = 20; //render effects on top of everything else
    
    lifetime = 0.3;
}

void Effect::onUpdate(float delta)
{
    lifetime -= delta;
    
    render_data.color = sp::Tween<sp::Color>::easeOutQuad(lifetime, 0.3, 0.0, sp::Color::White, sp::Color(255, 255, 255, 0));
    render_data.scale = sp::Tween<sp::Vector3f>::easeOutQuad(lifetime, 0.3, 0.0, sp::Vector3f(scale * 0.1, scale * 0.1, 1.0), sp::Vector3f(scale * 2.0, scale * 2.0, 1.0));
    
    if (lifetime < 0.0)
        delete this;
}

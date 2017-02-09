#include "blood.h"
#include "main.h"
#include <sp2/tween.h>
#include <sp2/random.h>
#include <sp2/graphics/spriteManager.h>

BloodDecal::BloodDecal(sp::Vector2d position)
: sp::SceneNode(::scene->getRoot())
{
    setPosition(position);
    setRotation(sp::random(0, 360));
    
    render_data = sp::SpriteManager::get("blood");
    render_data.type = sp::RenderData::Type::Additive;
    render_data.order = -1; //render blood below walls, enemies and players, but above floors
    
    lifetime = 60.0;
}

void BloodDecal::onUpdate(float delta)
{
    lifetime -= delta;
    
    render_data.color = sp::Tween<sp::Color>::easeOutQuad(lifetime, 60.0, 0.0, sp::Color::Red, sp::Color(255, 0, 0, 0));
    render_data.scale = sp::Tween<float>::easeOutQuad(lifetime, 60.0, 0.0, 1.0, 0.1);
    
    if (lifetime < 0.0)
        delete this;
}

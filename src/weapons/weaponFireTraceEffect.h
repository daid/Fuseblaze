#ifndef WEAPON_FIRE_TRACE_EFFECT_H
#define WEAPON_FIRE_TRACE_EFFECT_H

#include <sp2/scene/node.h>

class WeaponFireTraceEffect : public sp::SceneNode
{
public:
    WeaponFireTraceEffect(sp::Vector2d start_position, sp::Vector2d end_position);
    
    virtual void onUpdate(float delta) override;

protected:
    float lifetime;
};

#endif//EFFECT_H

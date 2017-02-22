#ifndef ENEMIES_SCREAMER_H
#define ENEMIES_SCREAMER_H

#include "enemy.h"

class Screamer : public Enemy
{
public:
    Screamer();
    
    virtual void onUpdate(float delta) override;
    virtual void onCollision(sp::CollisionInfo& info) override;
private:
    enum class State
    {
        Idle,
        Screaming,
        Charging
    } state;
    sp::Vector2d charge_normal;
    float charge_cooldown;
};

#endif//ENEMIES_SCREAMER_H

#include "screamer.h"
#include "../effect.h"

#include <sp2/random.h>

Screamer::Screamer()
{
    vision_range = 28.0;
    health = 5.0;
    movement_speed = 100.0;

    render_data.color = sp::HsvColor(sp::random(300, 320), 70, 50);

    state = State::Idle;
    charge_cooldown = 0.0;
}

void Screamer::onUpdate(float delta)
{
    if (attack_cooldown > 0.0)
        attack_cooldown -= delta;
    if (charge_cooldown > 0.0)
        charge_cooldown -= delta;
    switch(state)
    {
    case State::Idle:
        updateTargetPlayer();
        if (target_player)
        {
            charge_normal = (target_player->getGlobalPosition2D() - getGlobalPosition2D()).normalized();
            
            double target_rotation = charge_normal.angle();
            setAngularVelocity(getAngularVelocity2D() * 0.5 + sp::angleDifference(getGlobalRotation2D(), target_rotation) * 5.0);

            if (sp::angleDifference(getGlobalRotation2D(), target_rotation) < 10.0)
            {
                new Effect(getGlobalPosition2D(), 5);
                state = State::Screaming;
                charge_cooldown = 0.5;
            }
        }
        break;
    case State::Screaming:
        if (charge_cooldown <= 0.0)
        {
            state = State::Charging;
            charge_cooldown = 5.0;
        }
        break;
    case State::Charging:
        {
            sp::Vector2d velocity = getLinearVelocity2D() + charge_normal * double(delta) * movement_speed;
            setLinearVelocity(velocity);
            
            if (sp::random(0, 100) < 20)
                new Effect(getGlobalPosition2D() + sp::Vector2d(sp::random(-0.5, 0.5), sp::random(-0.5, 0.5)), 0.5);
            
            if (charge_cooldown <= 0.0)
            {
                state = State::Idle;
            }
        }
        break;
    }
}

void Screamer::onCollision(sp::CollisionInfo& info)
{
    if (state == State::Charging && sp::P<Wall>(info.other))
    {
        charge_cooldown -= 1.0;
    }
    Enemy::onCollision(info);
}

#ifndef ENEMY_H
#define ENEMY_H

#include "../wall.h"
#include "../player.h"

class Enemy : public sp::Node
{
public:
    Enemy();
    
    virtual void onUpdate(float delta) override;
    virtual void onCollision(sp::CollisionInfo& info) override;

    virtual void takeDamage(double amount);
protected:
    float vision_range;
    double movement_speed;
    sp::P<Player> target_player;
    sp::P<Player> last_seen_player;
    sp::Vector2d last_seen_player_position;
    float health;
    float attack_cooldown;

    void updateTargetPlayer();
};

#endif//ENEMY_H

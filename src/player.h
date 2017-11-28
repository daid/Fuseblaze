#ifndef PLAYER_H
#define PLAYER_H

class Weapon;

#include <sp2/scene/node.h>

class Pickup;
class Player : public sp::Node
{
public:
    static sp::PList<Player> players;

    Player(int index);
    
    virtual void onFixedUpdate() override;
    
    virtual void onUpdate(float delta) override;
    virtual void onCollision(sp::CollisionInfo& info) override;

    void takeDamage(float amount);

    int index;
    
    float hp;
    float max_hp;
    
    sp::P<Weapon> weapon;
    sp::string alternative_weapon;
    
    sp::P<Pickup> touching_pickup;
    int touching_pickup_remove_delay;
};

#endif//PLAYER_H

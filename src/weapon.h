#ifndef WEAPON_H
#define WEAPON_H

class Enemy;
class Player;

#include <sp2/scene/node.h>

class Weapon : public sp::SceneNode
{
public:
    Weapon(sp::P<Player> player);
    
    virtual void onUpdate(float delta);
    virtual void fire();
    virtual void launchProjectile();
private:
    sp::P<sp::SceneNode> aiming_node;
    
    float cooldown;

protected:
    float cooldown_delay;
};

#endif//WEAPON_H

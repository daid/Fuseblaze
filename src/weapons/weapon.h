#ifndef WEAPON_H
#define WEAPON_H

class Enemy;
class Player;

#include <sp2/scene/node.h>
#include "weaponInfo.h"

class Weapon : public sp::SceneNode
{
public:
    Weapon(WeaponInfo& info, sp::P<Player> player);
    
    virtual void onUpdate(float delta) override;
    virtual void fire();
    virtual void reload();
    virtual void launchProjectile();
    virtual void applyDamage(sp::Vector2d hit_location, sp::Vector2d hit_normal, sp::P<Enemy> enemy);
    
    int getAmmoCount();
    int getAmmoMax();
    
    WeaponInfo& info;
private:
    sp::P<sp::SceneNode> aiming_node;
    
    float cooldown;
    int ammo;
};

#endif//WEAPON_H

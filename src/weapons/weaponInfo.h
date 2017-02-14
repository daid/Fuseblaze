#ifndef WEAPON_INFO_H
#define WEAPON_INFO_H

#include <sp2/string.h>

class WeaponInfo
{
public:
    sp::string name;
    bool auto_fire; //Auto fire weapons keep firing when the button is done. Else it fires on key-up.
    
    int ammo_per_clip;
    float cooldown_delay;
    float reload_delay;
    
    int projectile_count;//Amount of projectiles fired per shot (or, when trace shot, amount of traces done)
    float projectile_spread;
    bool trace_shot;    //Do not launch a projectile, but trace to see what was hit.
    bool pass_trough_enemies; //When hitting enemies, continue with the trace or projectile.
    
    float damage_per_hit;
    float range;
    
    bool auto_aim;  //Auto aim at enemies within range and view angle.
    
    static void init();
    static WeaponInfo& get(sp::string name);
};

#endif//WEAPON_INFO_H


#ifndef PICKUPS_WEAPON_H
#define PICKUPS_WEAPON_H

#include "pickup.h"

class WeaponPickup : public Pickup
{
public:
    WeaponPickup();
    
    virtual void pickUpBy(sp::P<Player> player) override;
};

#endif//PICKUPS_WEAPON_H

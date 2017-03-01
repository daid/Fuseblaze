#include "weapon.h"
#include "../weapons/weapon.h"

#include "../player.h"

WeaponPickup::WeaponPickup()
{
    render_data.color = sp::HsvColor(30, 100, 100);
    
    name = WeaponInfo::random().name;
}

void WeaponPickup::pickUpBy(sp::P<Player> player)
{
    if (player->alternative_weapon == "")
    {
        if (player->weapon->info.name == name)
            return;
        player->alternative_weapon = name;
        Pickup::pickUpBy(player);
        return;
    }
    
}

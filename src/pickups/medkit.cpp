#include "medkit.h"

#include "../player.h"

Medkit::Medkit()
{
    render_data.color = sp::Color(1,0,0);
    setRotation(45);
    
    name = "Medkit";
}

void Medkit::pickUpBy(sp::P<Player> player)
{
    player->hp = std::min(player->max_hp, player->hp + 3.0f);
    Pickup::pickUpBy(player);
}

#include "keys.h"

PlayerKeys* player_keys[2];

PlayerKeys::PlayerKeys(int index)
: up("player_up_" + sp::string(index))
, down("player_down_" + sp::string(index))
, left("player_left_" + sp::string(index))
, right("player_right_" + sp::string(index))
, start("player_start_" + sp::string(index))
, fire("player_fire_" + sp::string(index))
, reload("player_reload_" + sp::string(index))
, pickup("player_pickup_" + sp::string(index))
, switch_weapon("player_switch_weapon_" + sp::string(index))
, unknown1("player_unknown1_" + sp::string(index))
, unknown2("player_unknown2_" + sp::string(index))
{
}

void PlayerKeys::init()
{
    player_keys[0] = new PlayerKeys(0);
    player_keys[0]->up.setKey("Up");
    player_keys[0]->down.setKey("Down");
    player_keys[0]->left.setKey("Left");
    player_keys[0]->right.setKey("Right");
    player_keys[0]->start.setKey("Num1");
    player_keys[0]->fire.setKey("Space");
    player_keys[0]->reload.setKey("Z");
    player_keys[0]->pickup.setKey("X");
    player_keys[0]->switch_weapon.setKey("C");
    player_keys[0]->unknown1.setKey("V");
    player_keys[0]->unknown2.setKey("B");
    
    player_keys[1] = new PlayerKeys(1);
    player_keys[1]->up.setKey("W");
    player_keys[1]->down.setKey("S");
    player_keys[1]->left.setKey("A");
    player_keys[1]->right.setKey("D");
    player_keys[1]->start.setKey("Num2");
    player_keys[1]->fire.setKey("Q");
    player_keys[1]->reload.setKey("E");
    player_keys[1]->pickup.setKey("R");
    player_keys[1]->switch_weapon.setKey("F");
    player_keys[1]->unknown1.setKey("T");
    player_keys[1]->unknown2.setKey("G");
}

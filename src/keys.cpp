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
    player_keys[0]->up.setKey(sf::Keyboard::Up);
    player_keys[0]->down.setKey(sf::Keyboard::Down);
    player_keys[0]->left.setKey(sf::Keyboard::Left);
    player_keys[0]->right.setKey(sf::Keyboard::Right);
    player_keys[0]->start.setKey(sf::Keyboard::Num1);
    player_keys[0]->fire.setKey(sf::Keyboard::Space);
    player_keys[0]->reload.setKey(sf::Keyboard::Z);
    player_keys[0]->pickup.setKey(sf::Keyboard::X);
    player_keys[0]->switch_weapon.setKey(sf::Keyboard::C);
    player_keys[0]->unknown1.setKey(sf::Keyboard::V);
    player_keys[0]->unknown2.setKey(sf::Keyboard::B);
    
    player_keys[1] = new PlayerKeys(1);
    player_keys[1]->up.setKey(sf::Keyboard::W);
    player_keys[1]->down.setKey(sf::Keyboard::S);
    player_keys[1]->left.setKey(sf::Keyboard::A);
    player_keys[1]->right.setKey(sf::Keyboard::D);
    player_keys[1]->start.setKey(sf::Keyboard::Num2);
    player_keys[1]->fire.setKey(sf::Keyboard::Q);
    player_keys[1]->reload.setKey(sf::Keyboard::E);
    player_keys[1]->pickup.setKey(sf::Keyboard::R);
    player_keys[1]->switch_weapon.setKey(sf::Keyboard::F);
    player_keys[1]->unknown1.setKey(sf::Keyboard::T);
    player_keys[1]->unknown2.setKey(sf::Keyboard::G);
}

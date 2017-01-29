#include "keys.h"

PlayerKeys* player_keys[2];

PlayerKeys::PlayerKeys(int index)
: up("player_up_" + sp::string(index))
, down("player_down_" + sp::string(index))
, left("player_left_" + sp::string(index))
, right("player_right_" + sp::string(index))
, start("player_start_" + sp::string(index))
, primary_fire("player_primary_fire_" + sp::string(index))
, secondary_fire("player_secondary_fire_" + sp::string(index))
, brake("player_brake_" + sp::string(index))
, hold_rotation("player_hold_rotation_" + sp::string(index))
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
    player_keys[0]->primary_fire.setKey(sf::Keyboard::Space);
    player_keys[0]->secondary_fire.setKey(sf::Keyboard::Z);
    //player_keys[0]->secondary_fire.setKey(sf::Keyboard::X);
    player_keys[0]->brake.setKey(sf::Keyboard::C);
    player_keys[0]->hold_rotation.setKey(sf::Keyboard::V);
    //player_keys[0]->secondary_fire.setKey(sf::Keyboard::B);
    
    player_keys[1] = new PlayerKeys(1);
    player_keys[1]->up.setKey(sf::Keyboard::W);
    player_keys[1]->down.setKey(sf::Keyboard::S);
    player_keys[1]->left.setKey(sf::Keyboard::A);
    player_keys[1]->right.setKey(sf::Keyboard::D);
    player_keys[1]->start.setKey(sf::Keyboard::Num2);
    player_keys[1]->primary_fire.setKey(sf::Keyboard::Q);
    player_keys[1]->secondary_fire.setKey(sf::Keyboard::E);
    //player_keys[1]->secondary_fire.setKey(sf::Keyboard::R);
    player_keys[1]->brake.setKey(sf::Keyboard::F);
    player_keys[1]->hold_rotation.setKey(sf::Keyboard::T);
    //player_keys[1]->secondary_fire.setKey(sf::Keyboard::G);
}

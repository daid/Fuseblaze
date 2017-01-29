#ifndef KEYS_H
#define KEYS_H

#include <sp2/io/keybinding.h>

class PlayerKeys : sf::NonCopyable
{
public:
    static void init();

    sp::io::Keybinding up;
    sp::io::Keybinding down;
    sp::io::Keybinding left;
    sp::io::Keybinding right;
    
    sp::io::Keybinding start;
    sp::io::Keybinding primary_fire;
    sp::io::Keybinding secondary_fire;
    sp::io::Keybinding brake;
    sp::io::Keybinding hold_rotation;
private:
    PlayerKeys(int index);
};

static constexpr int max_players = 2;
extern PlayerKeys* player_keys[max_players];

#endif//KEYS_H

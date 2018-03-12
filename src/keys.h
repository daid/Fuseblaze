#ifndef KEYS_H
#define KEYS_H

#include <sp2/io/keybinding.h>

class PlayerKeys : sp::NonCopyable
{
public:
    static void init();

    sp::io::Keybinding up;
    sp::io::Keybinding down;
    sp::io::Keybinding left;
    sp::io::Keybinding right;
    
    sp::io::Keybinding start;
    
    sp::io::Keybinding fire;
    sp::io::Keybinding reload;
    sp::io::Keybinding pickup;
    sp::io::Keybinding switch_weapon;
    sp::io::Keybinding unknown1;
    sp::io::Keybinding unknown2;
private:
    PlayerKeys(int index);
};

static constexpr int max_players = 2;
extern PlayerKeys* player_keys[max_players];

#endif//KEYS_H

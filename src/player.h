#ifndef PLAYER_H
#define PLAYER_H

class Weapon;

#include <sp2/scene/node.h>

class Player : public sp::SceneNode
{
public:
    static sp::PList<Player> players;

    Player(int index);
    
    virtual void onFixedUpdate() override;
    
    virtual void onUpdate(float delta) override;

    int index;
    
    sp::P<Weapon> weapon;
    sp::string alternative_weapon;
};

#endif//PLAYER_H

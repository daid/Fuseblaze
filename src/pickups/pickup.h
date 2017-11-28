#ifndef PICKUP_H
#define PICKUP_H

#include <sp2/scene/node.h>

class Player;
class Pickup : public sp::Node
{
public:
    Pickup();

    virtual void pickUpBy(sp::P<Player> player);

    sp::string name;
};

#endif//PICKUP_H

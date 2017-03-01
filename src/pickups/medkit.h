#ifndef MEDKIT_H
#define MEDKIT_H

#include "pickup.h"

class Medkit : public Pickup
{
public:
    Medkit();
    
    virtual void pickUpBy(sp::P<Player> player) override;
};

#endif//MEDKIT_H

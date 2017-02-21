#ifndef ENEMIES_HULK_H
#define ENEMIES_HULK_H

#include "enemy.h"

class Hulk : public Enemy
{
public:
    Hulk();
    
    virtual void takeDamage(double amount) override;
};

#endif//ENEMIES_HULK_H

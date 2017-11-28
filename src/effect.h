#ifndef EFFECT_H
#define EFFECT_H

#include <sp2/scene/node.h>

class Effect : public sp::Node
{
public:
    Effect(sp::Vector2d position, float scale);
    
    virtual void onUpdate(float delta) override;

protected:
    float lifetime;
    float scale;
};

#endif//EFFECT_H

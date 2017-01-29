#ifndef EFFECT_H
#define EFFECT_H

#include <sp2/scene/node.h>

class Effect : public sp::SceneNode
{
public:
    Effect(sp::Vector2d position);
    
    virtual void onUpdate(float delta) override;

protected:
    float lifetime;
};

#endif//EFFECT_H

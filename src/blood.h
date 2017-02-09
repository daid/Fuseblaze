#ifndef BLOOD_H
#define BLOOD_H

#include <sp2/scene/node.h>

class BloodDecal : public sp::SceneNode
{
public:
    BloodDecal(sp::Vector2d position);
    
    virtual void onUpdate(float delta) override;

protected:
    float lifetime;
};

#endif//BLOOD_H

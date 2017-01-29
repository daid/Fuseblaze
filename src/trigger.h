#ifndef TRIGGER_H
#define TRIGGER_H

#include <functional>
#include <sp2/scene/node.h>

class Trigger : public sp::SceneNode
{
public:
    std::function<void()> onTriggerEnter;
    std::function<void()> onTriggerExit;

    Trigger(sp::Vector2d position, double rotation, sp::Vector2f size);
    virtual void onCollision(sp::CollisionInfo& info) override;
    virtual void onFixedUpdate() override;
private:
    int touch_count;
};

#endif//TRIGGER_H

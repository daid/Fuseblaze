#ifndef TRIGGER_H
#define TRIGGER_H

#include <functional>
#include <sp2/scene/node.h>
#include <sp2/script/callback.h>

class Trigger : public sp::SceneNode
{
public:
    sp::script::Callback onTriggerEnter;
    sp::script::Callback onTriggerExit;

    Trigger(sp::Vector2d position, double rotation, sp::Vector2f size);
    virtual void onCollision(sp::CollisionInfo& info) override;
    virtual void onFixedUpdate() override;
    
    virtual void onRegisterScriptBindings(sp::ScriptBindingClass& script_binding_class) override;
private:
    int touch_count;
};

#endif//TRIGGER_H

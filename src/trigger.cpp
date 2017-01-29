#include "trigger.h"
#include "main.h"
#include "player.h"

#include <sp2/collision/2d/box.h>

Trigger::Trigger(sp::Vector2d position, double rotation, sp::Vector2f size)
: sp::SceneNode(::scene->getRoot())
{
    setPosition(position);
    setRotation(rotation);
    
    sp::collision::Box2D box(size.x, size.y);
    box.type = sp::collision::Shape::Type::Sensor;
    setCollisionShape(box);
    
    touch_count = 0;
}

void Trigger::onCollision(sp::CollisionInfo& info)
{
    sp::P<Player> player = info.other;
    if (player)
    {
        if (touch_count == 0 && onTriggerEnter)
            onTriggerEnter();
        touch_count = 2;
    }
}

void Trigger::onFixedUpdate()
{
    if (touch_count > 0)
    {
        touch_count--;
        if (!touch_count && onTriggerExit)
            onTriggerExit();
    }
}

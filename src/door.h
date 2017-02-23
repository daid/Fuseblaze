#ifndef DOOR_H
#define DOOR_H

#include "wall.h"

class Door : public Wall
{
public:
    Door(sp::Vector2d position, double rotation, sp::Vector2f size, sp::Color color)
    : Wall(position, rotation, size, color)
    {
        closed_position = position;
        open_position = position + (sp::Quaterniond::fromAngle(getGlobalRotation2D()) * sp::Vector2d(0.0, size.y));
        request_state = 0;
        actual_state = 0;
        movement_speed = 2.0;
        
        render_data.order -= 1; //Render doors below walls.
    }
    
    virtual void onUpdate(float delta)
    {
        if (request_state != actual_state)
        {
            if (actual_state < request_state)
            {
                actual_state += movement_speed * delta;
                if (actual_state >= request_state)
                {
                    actual_state = request_state;
                    onOpened.call();
                }
            }
            if (actual_state > request_state)
            {
                actual_state -= movement_speed * delta;
                if (actual_state <= request_state)
                {
                    actual_state = request_state;
                    onClosed.call();
                }
            }
            setPosition(closed_position + (open_position - closed_position) * actual_state);
        }
    }
    
    void open()
    {
        request_state = 1;
    }
    
    void close()
    {
        request_state = 0;
    }
    
    void setMovementSpeed(double speed)
    {
        movement_speed = speed;
    }
    
    virtual void onRegisterScriptBindings(sp::ScriptBindingClass& script_binding_class) override
    {
        Wall::onRegisterScriptBindings(script_binding_class);
        
        script_binding_class.bind("open", &Door::open);
        script_binding_class.bind("setMovementSpeed", &Door::setMovementSpeed);
        script_binding_class.bind("close", &Door::close);
        script_binding_class.bind("onOpened", onOpened);
        script_binding_class.bind("onClosed", onClosed);
    }
private:
    double movement_speed;
    double request_state;
    double actual_state;
    sp::Vector2d open_position;
    sp::Vector2d closed_position;
    
    sp::script::Callback onOpened;
    sp::script::Callback onClosed;
};

#endif//DOOR_H

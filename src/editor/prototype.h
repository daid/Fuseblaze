#ifndef EDITOR_PROTOTYPE_H
#define EDITOR_PROTOTYPE_H

#include <sp2/scene/node.h>
#include "../prefab.h"

class Prototype : public sp::Node
{
public:
    Prototype(Prefab::Part::Type type, sp::Vector2d position, double rotation, sp::Vector2f size);
        
    void setSize(sp::Vector2f size);
    void setColor(sp::HsvColor color);
    
    virtual void createHandles();
    
    virtual void destroyHandles()
    {
        for(Node* child : getChildren())
            delete child;
    }
    
    virtual sp::P<sp::Node> getHandle(sp::Vector2d position)
    {
        for(Node* child : getChildren())
        {
            if ((child->getGlobalPosition2D() - position).length() < 0.5)
            {
                return child;
            }
        }
        return nullptr;
    }

    Prefab::Part::Type type;
    sp::Vector2f size;
    sp::HsvColor color;
    sp::string id;
};

#endif//EDITOR_PROTOTYPE_H

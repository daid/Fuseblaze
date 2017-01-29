#ifndef EDITOR_PROTOTYPE_H
#define EDITOR_PROTOTYPE_H

#include <sp2/scene/node.h>
#include "../prefab.h"

class Prototype : public sp::SceneNode
{
public:
    Prototype(Prefab::Part::Type type, sp::Vector2d position, double rotation, sp::Vector2f size);
        
    void setSize(sp::Vector2f size);
    void setColor(sp::HsvColor color);
    
    virtual void createHandles();
    
    virtual void destroyHandles()
    {
        for(SceneNode* child : getChildren())
            delete child;
    }
    
    virtual sp::P<sp::SceneNode> getHandle(sp::Vector2d position)
    {
        for(SceneNode* child : getChildren())
        {
            if (sp::length(child->getGlobalPosition2D() - position) < 0.5)
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

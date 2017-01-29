#ifndef PLAYER_H
#define PLAYER_H

class Player;

#include "weapon.h"
#include "main.h"
#include "keys.h"
#include "shadowCastNode.h"
#include <sp2/graphics/meshdata.h>
#include <sp2/collision/2d/circle.h>

class Player : public sp::SceneNode
{
public:
    static sp::PList<Player> players;

    Player(int index)
    : sp::SceneNode(::scene->getRoot()), index(index)
    {
        render_data.type = sp::RenderData::Type::Normal;
        render_data.shader = sp::Shader::get("shader/color.shader");
        render_data.mesh = sp::MeshData::createQuad(sp::Vector2f(1, 1));
        render_data.color = sp::HsvColor(0, 70, 50);
        
        sp::collision::Circle2D circle(0.5);
        setCollisionShape(circle);
        setPosition(sp::Vector2d(0, 0));
        
        //Forward field of view shadow
        new ShadowCastNode(this, index, 0.1, 110.0);
        //Maximum view distance shadow
        new ShadowCastNode(this, index, 20, 0.0);
        
        weapon = new Weapon(this);
        
        players.add(this);
    }
    
    virtual void onFixedUpdate()
    {
        double target_rotation;
        sp::Vector2d movement_request(0.0, 0.0);
        movement_request.x += player_keys[index]->up.getValue();
        movement_request.x -= player_keys[index]->down.getValue();
        movement_request.y += player_keys[index]->left.getValue();
        movement_request.y -= player_keys[index]->right.getValue();
        
        if (movement_request != sp::Vector2d(0.0, 0.0))
        {
            setLinearVelocity(movement_request * 10.0);
            target_rotation = sp::toRotationAngle(movement_request);
        }
        else
        {
            setLinearVelocity(getLinearVelocity2D() * 0.5);
            target_rotation = getGlobalRotation2D();
        }
        
        if (player_keys[index]->primary_fire.get())
        {
            target_rotation = getGlobalRotation2D();
        }
        
        setAngularVelocity(getAngularVelocity2D() * 0.5 + sp::angleDifference(getGlobalRotation2D(), target_rotation) * 5.0);
    }
    
    virtual void onUpdate(float delta)
    {
        if (player_keys[index]->primary_fire.getUp())
        {
            weapon->fire();
        }
    }

private:
    int index;
    
    sp::P<Weapon> weapon;
};

#endif//PLAYER_H

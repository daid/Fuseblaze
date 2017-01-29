#ifndef ENEMY_H
#define ENEMY_H

#include "wall.h"

class Enemy : public sp::SceneNode
{
public:
    Enemy()
    : sp::SceneNode(::scene->getRoot())
    {
        vision_range = 25.0;

        render_data.type = sp::RenderData::Type::Normal;
        render_data.shader = sp::Shader::get("shader/color.shader");
        render_data.mesh = sp::MeshData::createQuad(sp::Vector2f(1, 1));
        render_data.color = sp::HsvColor(200, 70, 50);
        
        sp::collision::Circle2D circle(0.5);
        circle.angular_damping = 5;
        circle.linear_damping = 5;
        setCollisionShape(circle);
        setPosition(sp::Vector2d(0, 0));
    }
    
    virtual void onUpdate(float delta)
    {
        updateTargetPlayer();
        
        if (target_player)
        {
            sp::Vector2d normal = sp::normalize(target_player->getGlobalPosition2D() - getGlobalPosition2D());
            
            sp::Vector2d velocity = getLinearVelocity2D() + normal * double(delta) * 10.0;
            setLinearVelocity(velocity);
            
            double target_rotation = sp::toRotationAngle(target_player->getGlobalPosition2D() - getGlobalPosition2D());
            setAngularVelocity(getAngularVelocity2D() * 0.5 + sp::angleDifference(getGlobalRotation2D(), target_rotation) * 5.0);
        }
    }

protected:
    float vision_range;

    void updateTargetPlayer()
    {
        sp::Vector2d position = getGlobalPosition2D();
        
        double target_distance = 0.0;
        if (target_player)
            target_distance = sp::length(position - target_player->getGlobalPosition2D());
        
        for(Player* player : Player::players)
        {
            sp::Vector2d player_position = player->getGlobalPosition2D();
            double distance = sp::length(position - player_position);
            if (distance > vision_range)
                continue;

            bool visible = true;
            ::scene->queryCollisionAny(position, player_position, [&visible](sp::P<sp::SceneNode> object, sp::Vector2d hit_location, sp::Vector2d hit_normal)
            {
                if (sp::P<Wall>(object))
                {
                    visible = false;
                    return false;
                }
                return true;
            });
            if (!visible)
            {
                if (player == *target_player)
                    target_player = nullptr;
                continue;
            }
            
            if (!target_player || distance < target_distance - 5.0)
            {
                target_player = player;
                target_distance = distance;
            }
        }
    }

    sp::P<Player> target_player;
};

#endif//ENEMY_H

#include "enemy.h"
#include "../effect.h"

#include <sp2/random.h>
#include <sp2/collision/2d/circle.h>

Enemy::Enemy()
: sp::SceneNode(::scene->getRoot())
{
    vision_range = 35.0;
    health = 3.0;
    movement_speed = 15.0;
    attack_cooldown = 0.0;

    render_data.type = sp::RenderData::Type::Normal;
    render_data.shader = sp::Shader::get("shader/color.shader");
    render_data.mesh = sp::MeshData::createQuad(sp::Vector2f(1, 1));
    render_data.color = sp::HsvColor(sp::random(190, 210), 70, 50);
    
    sp::collision::Circle2D circle(0.5);
    circle.angular_damping = 5;
    circle.linear_damping = 5;
    setCollisionShape(circle);
    setPosition(sp::Vector2d(0, 0));
}

void Enemy::onUpdate(float delta)
{
    if (attack_cooldown > 0.0)
        attack_cooldown -= delta;
    updateTargetPlayer();
    
    if (target_player)
    {
        last_seen_player_position = target_player->getGlobalPosition2D();
        last_seen_player = target_player;
    }
    
    if (last_seen_player)
    {
        sp::Vector2d diff = last_seen_player_position - getGlobalPosition2D();
        sp::Vector2d normal = sp::normalize(diff);
        
        if (sp::length(diff) < 1.0)
        {
            last_seen_player = nullptr;
        }else{
            sp::Vector2d velocity = getLinearVelocity2D() + normal * double(delta) * movement_speed;
            setLinearVelocity(velocity);
            
            double target_rotation = sp::toRotationAngle(diff);
            setAngularVelocity(getAngularVelocity2D() * 0.5 + sp::angleDifference(getGlobalRotation2D(), target_rotation) * 5.0);
        }
    }
}

void Enemy::onCollision(sp::CollisionInfo& info)
{
    if (attack_cooldown > 0.0)
        return;
    sp::P<Player> player = info.other;
    if (player)
    {
        new Effect(info.position, 1.0);
        player->setLinearVelocity(info.normal * 10.0);
        player->takeDamage(1.0);
        
        attack_cooldown = 0.5;
    }
}

void Enemy::takeDamage(double amount)
{
    health -= amount;
    if (health <= 0.0)
        delete this;
}

void Enemy::updateTargetPlayer()
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

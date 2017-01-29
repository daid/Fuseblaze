#include "weapon.h"
#include "player.h"
#include "enemy.h"
#include "effect.h"
#include <sp2/graphics/spriteManager.h>
#include <sp2/scene/scene.h>


Weapon::Weapon(sp::P<Player> player)
: sp::SceneNode(player)
{
    //Aiming laser
    aiming_node = new sp::SceneNode(this);
    aiming_node->render_data = sp::SpriteManager::get("aim_laser");
    aiming_node->render_data.type = sp::RenderData::Type::Additive;
    aiming_node->render_data.color = sp::Color::Red;
    aiming_node->render_data.order = -1; //render the laser below normal objects, but above floors.
    aiming_node->setRotation(-90);
    aiming_node->setPosition(sp::Vector2d(7.5, 0));
    
    cooldown = 0.0;
    cooldown_delay = 0.1;
}

void Weapon::onUpdate(float delta)
{
    double target_distance = 0.0;
    double target_angle = 0.0;
    double angle = getParent()->getGlobalRotation2D();
    sp::P<Enemy> target;
    getScene()->queryCollision(getGlobalPosition2D(), 20, [this, angle, &target, &target_distance, &target_angle](sp::P<sp::SceneNode> object)
    {
        sp::P<Enemy> enemy = object;
        if (enemy)
        {
            double distance = sp::length(getGlobalPosition2D() - object->getGlobalPosition2D());
            if (!target || distance < target_distance)
            {
                double enemy_angle = sp::toRotationAngle(enemy->getGlobalPosition2D() - getGlobalPosition2D()) - angle;
                if (std::abs(sp::angleDifference(enemy_angle, 0.0)) < 55)
                {
                    bool visible = true;
                    ::scene->queryCollisionAny(getGlobalPosition2D(), enemy->getGlobalPosition2D(), [&visible](sp::P<sp::SceneNode> object, sp::Vector2d hit_location, sp::Vector2d hit_normal)
                    {
                        if (sp::P<Wall>(object))
                        {
                            visible = false;
                            return false;
                        }
                        return true;
                    });
                    if (visible)
                    {
                        target = enemy;
                        target_distance = distance;
                        target_angle = enemy_angle;
                    }
                }
            }
        }
        return true;
    });
    setRotation(target_angle);
    
    if (cooldown > 0.0)
    {
        cooldown -= delta;
        
        if (cooldown <= 0.0)
        {
            aiming_node->render_data.color = sp::Color::Red;
        }
    }
}

void Weapon::fire()
{
    if (cooldown <= 0.0)
    {
        cooldown = cooldown_delay;
        aiming_node->render_data.color = sp::HsvColor(0, 0, 50);
        
        launchProjectile();
    }
}

void Weapon::launchProjectile()
{
    sp::Vector2d position = getGlobalPosition2D();
    ::scene->queryCollisionAll(position, position + sp::Quaterniond::fromAngle(getGlobalRotation2D()) * sp::Vector2d(20.0, 0.0), [](sp::P<sp::SceneNode> object, sp::Vector2d hit_location, sp::Vector2d hit_normal)
    {
        if (sp::P<Wall>(object))
        {
            new Effect(hit_location);
            return false;
        }
        if (sp::P<Enemy>(object))
        {
            new Effect(hit_location);
            object->setLinearVelocity(hit_normal * -10.0);
            //delete *object;
            return false;
        }
        return true;
    });
}

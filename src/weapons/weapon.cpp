#include "weapon.h"
#include "weaponFireTraceEffect.h"
#include "../player.h"
#include "../enemies/enemy.h"
#include "../effect.h"
#include "../blood.h"
#include <sp2/scene/scene.h>
#include <sp2/random.h>
#include <sp2/graphics/meshData.h>
#include <sp2/graphics/textureManager.h>

Weapon::Weapon(WeaponInfo& info, sp::P<Player> player)
: sp::Node(player), info(info)
{
    //Aiming laser
    aiming_node = new sp::Node(this);
    aiming_node->render_data.mesh = sp::MeshData::createQuad(sp::Vector2f(5, 15));
    aiming_node->render_data.texture = sp::texture_manager.get("weapons/aim_laser.png");
    aiming_node->render_data.shader = sp::Shader::get("internal:basic.shader");
    aiming_node->render_data.type = sp::RenderData::Type::Additive;
    aiming_node->render_data.color = sp::Color(1,0,0);
    aiming_node->render_data.order = -1; //render the laser below normal objects, but above floors.
    aiming_node->setRotation(-90);
    aiming_node->setPosition(sp::Vector2d(7.5, 0));
    
    cooldown = info.reload_delay;
    
    ammo = 0;
}

void Weapon::onUpdate(float delta)
{
    if (info.auto_aim)
    {
        double target_distance = 0.0;
        double target_angle = 0.0;
        double angle = getParent()->getGlobalRotation2D();
        sp::P<Enemy> target;
        getScene()->queryCollision(getGlobalPosition2D(), info.range, [this, angle, &target, &target_distance, &target_angle](sp::P<sp::Node> object)
        {
            sp::P<Enemy> enemy = object;
            if (enemy)
            {
                double distance = (getGlobalPosition2D() - object->getGlobalPosition2D()).length();
                if (!target || distance < target_distance)
                {
                    double enemy_angle = (enemy->getGlobalPosition2D() - getGlobalPosition2D()).angle() - angle;
                    if (std::abs(sp::angleDifference(enemy_angle, 0.0)) < 55)
                    {
                        bool visible = true;
                        ::scene->queryCollisionAny(sp::Ray2d(getGlobalPosition2D(), enemy->getGlobalPosition2D()), [&visible, &target](sp::P<sp::Node> object, sp::Vector2d hit_location, sp::Vector2d hit_normal)
                        {
                            if (sp::P<Wall>(object) || (object->isSolid() && !sp::P<Enemy>(object)))
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
    }
    
    if (cooldown > 0.0)
    {
        cooldown -= delta;
        
        if (cooldown <= 0.0)
        {
            if (ammo == 0)
                ammo = info.ammo_per_clip;
            aiming_node->render_data.color = sp::Color(1,0,0);
        }
    }
}

void Weapon::fire()
{
    if (cooldown <= 0.0)
    {
        aiming_node->render_data.color = sp::HsvColor(0, 0, 50);
        
        for(int n=0; n<info.projectile_count; n++)
            launchProjectile();
        ammo--;
        if (ammo == 0)
            cooldown += info.reload_delay;
        else
            cooldown += info.cooldown_delay;
    }
}

void Weapon::reload()
{
    if (cooldown <= 0.0 && ammo < info.ammo_per_clip)
    {
        aiming_node->render_data.color = sp::HsvColor(0, 0, 50);

        ammo = 0;
        cooldown = info.reload_delay;
    }
}

void Weapon::launchProjectile()
{
    float angle = getGlobalRotation2D() + sp::random(-info.projectile_spread / 2.0, info.projectile_spread / 2.0);

    sp::Vector2d position = getGlobalPosition2D();
    sp::Vector2d end_position = position + sp::Quaterniond::fromAngle(angle) * sp::Vector2d(info.range, 0.0);
    bool hit = false;
    ::scene->queryCollisionAll(sp::Ray2d(position, end_position), [this, &hit](sp::P<sp::Node> object, sp::Vector2d hit_location, sp::Vector2d hit_normal)
    {
        sp::P<Enemy> enemy = object;
        if (enemy)
        {
            new Effect(hit_location, 1.0);
            if (!info.pass_trough_enemies)
                new WeaponFireTraceEffect(getGlobalPosition2D(), hit_location);
            applyDamage(hit_location, hit_normal, enemy);
            hit = true;
            return info.pass_trough_enemies;
        }
        if (sp::P<Wall>(object) || object->isSolid())
        {
            new Effect(hit_location, 1.0);
            new WeaponFireTraceEffect(getGlobalPosition2D(), hit_location);
            hit = true;
            return false;
        }
        return true;
    });
    if (!hit)
    {
        new WeaponFireTraceEffect(getGlobalPosition2D(), end_position);
    }
}

void Weapon::applyDamage(sp::Vector2d hit_location, sp::Vector2d hit_normal, sp::P<Enemy> enemy)
{
    enemy->setLinearVelocity(enemy->getLinearVelocity2D() + hit_normal * -10.0);
    enemy->takeDamage(info.damage_per_hit);
    
    sp::Vector2d offset = sp::Quaterniond::fromAngle(getGlobalRotation2D() + sp::random(-5, 5)) * sp::Vector2d(0.5, 0.0);
    for(int n=0; n<5; n++)
    {
        new BloodDecal(hit_location + offset * double(n) + sp::Vector2d(sp::random(-0.5, 0.5), sp::random(-0.5, 0.5)));
    }
}

int Weapon::getAmmoCount()
{
    return ammo;
}

int Weapon::getAmmoMax()
{
    return info.ammo_per_clip;
}

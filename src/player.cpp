#include "player.h"

#include "weapons/weapon.h"
#include "main.h"
#include "keys.h"
#include "shadowCastNode.h"
#include <sp2/graphics/meshdata.h>
#include <sp2/collision/2d/circle.h>

sp::PList<Player> Player::players;

Player::Player(int index)
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
    new ShadowCastNode(this, index, 1.0, 110.0);
    //Maximum view distance shadow
    new ShadowCastNode(this, index, 30);
    
    weapon = new Weapon(WeaponInfo::get("Shotgun"), this);
    alternative_weapon = "Minigun";

    max_hp = hp = 10.0;
    
    players.add(this);
}

void Player::onFixedUpdate()
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
    
    if (player_keys[index]->fire.get())
    {
        if (!weapon->info.auto_fire)
            target_rotation = getGlobalRotation2D();
        else
            weapon->fire();
    }
    
    setAngularVelocity(getAngularVelocity2D() * 0.5 + sp::angleDifference(getGlobalRotation2D(), target_rotation) * 5.0);
}

void Player::onUpdate(float delta)
{
    if (player_keys[index]->switch_weapon.getUp())
    {
        sp::string old_weapon = weapon->info.name;
        delete *weapon;
        weapon = new Weapon(WeaponInfo::get(alternative_weapon), this);
        alternative_weapon = old_weapon;
    }
    if (player_keys[index]->fire.getUp() && !weapon->info.auto_fire)
    {
        weapon->fire();
    }
    if (player_keys[index]->reload.getUp())
    {
        weapon->reload();
    }
}

void Player::takeDamage(float amount)
{
    hp -= amount;
    if (hp <= 0.0)
        delete this;
}

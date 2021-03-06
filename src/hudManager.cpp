#include "hudManager.h"
#include "player.h"
#include "main.h"
#include "weapons/weapon.h"
#include "pickups/pickup.h"

#include <sp2/graphics/gui/loader.h>
#include <sp2/graphics/gui/widget/label.h>
#include <sp2/graphics/gui/widget/progressbar.h>

HudManager::HudManager(sp::P<sp::Node> root)
: sp::Node(root)
{
    huds[0] = sp::gui::Loader::load("gui/hud.gui", "HUD", gui_scene->getRoot());
    huds[1] = sp::gui::Loader::load("gui/hud.gui", "HUD", gui_scene->getRoot());
    
    for(auto child : huds[1]->getChildren())
    {
        sp::P<sp::gui::Widget> widget = sp::P<sp::Node>(child);
        if (widget->layout.alignment == sp::Alignment::BottomLeft)
            widget->layout.alignment = sp::Alignment::BottomRight;
        if (widget->layout.alignment == sp::Alignment::Left)
            widget->layout.alignment = sp::Alignment::Right;
        if (widget->layout.alignment == sp::Alignment::TopLeft)
            widget->layout.alignment = sp::Alignment::TopRight;
    }
}

void HudManager::onUpdate(float delta)
{
    huds[0]->hide();
    huds[1]->hide();
    for(sp::P<Player> player : Player::players)
    {
        update(player, huds[player->index]);
    }
}

void HudManager::update(sp::P<Player> player, sp::P<sp::gui::Widget> hud)
{
    hud->show();

    sp::P<sp::gui::Progressbar> bar;
    sp::P<sp::gui::Label> label;

    bar = hud->getWidgetWithID("AMMO");
    if (bar)
    {
        bar->setValue(player->weapon->getAmmoCount());
        bar->setRange(0, player->weapon->getAmmoMax());
    }
    label = hud->getWidgetWithID("WEAPON_LABEL");
    if (label)
        label->setLabel(player->weapon->info.name);

    label = hud->getWidgetWithID("ALT_WEAPON_LABEL");
    if (label)
        label->setLabel(player->alternative_weapon);

    bar = hud->getWidgetWithID("HEALTH");
    if (bar)
    {
        bar->setValue(player->hp);
        bar->setRange(0, player->max_hp);
    }
}

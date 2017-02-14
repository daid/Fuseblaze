#include "hudManager.h"
#include "player.h"
#include "main.h"
#include "weapons/weapon.h"

#include <sp2/graphics/gui/guiLoader.h>
#include <sp2/graphics/gui/widget/label.h>
#include <sp2/graphics/gui/widget/progressbar.h>

HudManager::HudManager(sp::P<sp::SceneNode> root)
: sp::SceneNode(root)
{
    huds[0] = sp::gui::Loader::load("gui/hud.gui", "HUD_LEFT", gui_layer->getRoot());
    huds[1] = sp::gui::Loader::load("gui/hud.gui", "HUD_RIGHT", gui_layer->getRoot());
}

void HudManager::onUpdate(float delta)
{
    huds[0]->hide();
    huds[1]->hide();
    for(Player* player : Player::players)
    {
        update(player, huds[player->index]);
    }
}

void HudManager::update(Player* player, sp::P<sp::gui::Widget> hud)
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
}
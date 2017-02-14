#ifndef HUD_MANAGER_H
#define HUD_MANAGER_H

#include <sp2/graphics/gui/widget/widget.h>
#include <sp2/scene/node.h>

class Player;
class HudManager : public sp::SceneNode
{
public:
    HudManager(sp::P<sp::SceneNode> root);
    
    virtual void onUpdate(float delta) override;

private:
    void update(Player* player, sp::P<sp::gui::Widget> hud);

    sp::P<sp::gui::Widget> huds[2];
};

#endif//HUD_MANAGER_H

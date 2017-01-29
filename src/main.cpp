#include <sp2/engine.h>
#include <sp2/window.h>
#include <sp2/logging.h>
#include <sp2/io/directoryResourceProvider.h>
#include <sp2/graphics/gui/theme.h>
#include <sp2/graphics/gui/guiLoader.h>
#include <sp2/graphics/gui/widget/button.h>
#include <sp2/graphics/gui/widget/slider.h>
#include <sp2/graphics/scene/graphicslayer.h>
#include <sp2/graphics/scene/basicnoderenderpass.h>
#include <sp2/graphics/scene/collisionrenderpass.h>
#include <sp2/graphics/meshdata.h>
#include <sp2/graphics/spriteManager.h>
#include <sp2/graphics/opengl.h>
#include <sp2/scene/scene.h>
#include <sp2/scene/cameraNode.h>
#include <sp2/collision/2d/box.h>
#include <sp2/collision/2d/circle.h>

#include <fstream>

#include "keys.h"
#include "wall.h"
#include "door.h"
#include "shadowRenderPass.h"
#include "player.h"
#include "floor.h"
#include "enemy.h"
#include "trigger.h"
#include "mapGenerator.h"
#include "editor/editor.h"
#include "editor/prototype.h"

sp::P<sp::Scene> scene;
sp::P<sp::gui::GraphicsLayer> gui_layer;
sp::P<sp::CameraNode> camera;
sp::P<sp::SceneGraphicsLayer> scene_layer;
sp::PList<Player> Player::players;

class Crate : public sp::SceneNode
{
public:
    Crate()
    : sp::SceneNode(::scene->getRoot())
    {
        render_data.type = sp::RenderData::Type::Normal;
        render_data.shader = sp::Shader::get("shader/color.shader");
        render_data.mesh = sp::MeshData::createQuad(sp::Vector2f(1, 1));
        render_data.color = sp::HsvColor(34, 80, 60);
        
        sp::collision::Box2D box(1, 1);
        box.angular_damping = 5;
        box.linear_damping = 5;
        setCollisionShape(box);
        
        //new ShadowCastNode(this, sp::Vector2f(1, 1));
    }
};

class CameraController : public sp::CameraNode
{
public:
    CameraController(sp::P<sp::SceneNode> parent)
    : sp::CameraNode(parent)
    {
        setOrtographic(25.0);
        setRotation(-90);
        setPosition(sp::Vector2d(5, 0));
    }

    virtual void onUpdate(float delta)
    {
        sp::Vector2d position;
        int player_count = 0;
        for(Player* p : Player::players)
        {
            position += p->getGlobalPosition2D();
            player_count++;
        }
        if (player_count > 0)
            position /= double(player_count);
        setPosition(position);
    }
};

int main(int argc, char** argv)
{
    //Create resource providers, so we can load things.
    new sp::io::DirectoryResourceProvider("resources");
    
    //Load our ui theme.
    sp::gui::Theme::loadTheme("default", "gui/theme/basic.theme.txt");
    
    //Create a window to render on, and our engine.
    new sp::Window(4.0/3.0);
    sp::P<sp::Engine> engine = new sp::Engine();
    
    //Initialize the player keys, needs to be done before keybindings are loaded.
    PlayerKeys::init();
    
    sp::SpriteManager::create("aim_laser", "weapons/aim_laser.png", sp::Vector2f(5, 15));
    sp::SpriteManager::create("effect_ring", "effect/ring.png", sp::Vector2f(1, 1));
    
    {
        scene = new sp::Scene();
    }
    
    sp::P<ShadowRenderPass> shadow_pass;
    {
        gui_layer = new sp::gui::GraphicsLayer(100);
        gui_layer->setMinimalVirtualSize(sf::Vector2f(1280, 800));
        gui_layer->setMaximumVirtualSize(sf::Vector2f(1280, 80000));

        camera = new CameraController(scene->getRoot());

        scene_layer = new sp::SceneGraphicsLayer(10);
        scene_layer->addRenderPass(new sp::BasicNodeRenderPass("window", scene, camera));
        shadow_pass = new ShadowRenderPass("window", scene, camera);
        scene_layer->addRenderPass(shadow_pass);
        //scene_layer->addRenderPass(new sp::CollisionRenderPass("window", scene, camera));
    }
    
    if (1)
    {
        shadow_pass->light_sources.add(new Player(0));
        //Player* p = new Player(1);
        //shadow_pass->light_sources.add(p);
        //p->setPosition(sp::Vector2d(1, 5));
        
        MapGenerator mg;
        mg.generate();
        
        /*
        new Wall(sp::Vector2d(5, 0), 30, sp::Vector2f(0.5, 5), sp::HsvColor(111, 52, 56));
        new Wall(sp::Vector2d(-5, 0), 0, sp::Vector2f(5, 5), sp::HsvColor(111, 52, 56));
        new Wall(sp::Vector2d(-5, 3), 0, sp::Vector2f(5, 5), sp::HsvColor(111, 52, 56));
        
        Door* door = new Door(sp::Vector2d(5, 5), 15, sp::Vector2f(0.5, 5), sp::HsvColor(111, 0, 56));
        
        new Floor(sp::Vector2d(0, 5), 0, sp::Vector2f(2, 2), sp::HsvColor(0, 41, 25));
        Trigger* trigger = new Trigger(sp::Vector2d(0, 5), 0, sp::Vector2f(2, 2));
        trigger->onTriggerEnter = [door]()
        {
            door->open();
        };
        trigger->onTriggerExit = [door]()
        {
            door->close();
        };

        new Floor(sp::Vector2d(0, 0), 10, sp::Vector2f(25, 25), sp::HsvColor(228, 41, 25));

        for(int n=0;n<2; n++)
            (new Enemy())->setPosition(sp::Vector2d(10, 10+0.1*n));

        for(int n=0; n<5; n++)
            (new Crate())->setPosition(sp::Vector2d(0, -2));
        */
    }
    else
    {
        new Editor(gui_layer->getRoot());
    }
    engine->run();
    
    return 0;
}

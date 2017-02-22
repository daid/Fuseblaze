#include <sp2/engine.h>
#include <sp2/window.h>
#include <sp2/logging.h>
#include <sp2/io/directoryResourceProvider.h>
#include <sp2/graphics/gui/theme.h>
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
#include "enemies/enemy.h"
#include "trigger.h"
#include "hudManager.h"
#include "mapGenerator.h"
#include "weapons/weaponInfo.h"
#include "editor/editor.h"
#include "editor/prototype.h"

sp::P<sp::Scene> scene;
sp::P<sp::gui::GraphicsLayer> gui_layer;
sp::P<sp::CameraNode> camera;
sp::P<sp::SceneGraphicsLayer> scene_layer;

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
        {
            position /= double(player_count);
            setPosition(position);
        }
#ifdef DEBUG
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
            setOrtographic(200.0);
        else
            setOrtographic(25.0);
#endif
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            sp::Engine::getInstance()->shutdown();
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
    WeaponInfo::init();
    
    sp::SpriteManager::create("aim_laser", "weapons/aim_laser.png", sp::Vector2f(5, 15));
    sp::SpriteManager::create("effect_ring", "effect/ring.png", sp::Vector2f(1, 1));
    sp::SpriteManager::create("weapon_trace", "weapons/trace.png", sp::Vector2f(1, 0.1));
    sp::SpriteManager::create("blood", "effect/blood.png", sp::Vector2f(0.5, 0.5));
    
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
        shadow_pass = new ShadowRenderPass("window", scene, camera);
        scene_layer->addRenderPass(shadow_pass);
#ifdef DEBUG
        //scene_layer->addRenderPass(new sp::CollisionRenderPass("window", scene, camera));
#endif
    }
    
    {
        new HudManager(scene->getRoot());
    }
    
    if (0)
    {
        new Editor(gui_layer->getRoot(), "prefab/medium_hallway_door");
    }
    else
    {
        Player* p = new Player(0);
        shadow_pass->light_sources.add(p);
        
        if (1)
        {
            Player* p = new Player(1);
            shadow_pass->light_sources.add(p);
            p->setPosition(sp::Vector2d(0.1, 0));
            p->setRotation(180);
        }
        MapGenerator mg;
        mg.generate();
    }
    engine->run();
    
    return 0;
}

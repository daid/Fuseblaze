#include <sp2/engine.h>
#include <sp2/window.h>
#include <sp2/logging.h>
#include <sp2/io/directoryResourceProvider.h>
#include <sp2/io/resourceProvider.h>
#include <sp2/graphics/gui/theme.h>
#include <sp2/graphics/scene/graphicslayer.h>
#include <sp2/graphics/scene/basicnoderenderpass.h>
#include <sp2/graphics/scene/collisionrenderpass.h>
#include <sp2/graphics/meshdata.h>
#include <sp2/graphics/opengl.h>
#include <sp2/graphics/textureManager.h>
#include <sp2/scene/scene.h>
#include <sp2/scene/camera.h>
#include <sp2/scene/tilemap.h>
#include <sp2/collision/2d/box.h>
#include <sp2/collision/2d/circle.h>
#include <sp2/random.h>

#include <fstream>
#include "dynamicSectorLoader.h"

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
#include "pickups/weapon.h"
#include "weapons/weaponInfo.h"
#include "editor/editor.h"
#include "editor/prototype.h"

sp::P<sp::Scene> scene;
sp::P<sp::gui::Scene> gui_scene;
sp::P<sp::Camera> camera;
sp::P<sp::SceneGraphicsLayer> scene_layer;

class Crate : public sp::Node
{
public:
    Crate()
    : sp::Node(::scene->getRoot())
    {
        render_data.type = sp::RenderData::Type::Normal;
        render_data.shader = sp::Shader::get("internal:color.shader");
        render_data.mesh = sp::MeshData::createQuad(sp::Vector2f(1, 1));
        render_data.color = sp::HsvColor(34, 80, 60);
        
        sp::collision::Box2D box(1, 1);
        box.angular_damping = 5;
        box.linear_damping = 5;
        setCollisionShape(box);
        
        //new ShadowCastNode(this, sp::Vector2f(1, 1));
    }
};

class CameraController : public sp::Camera
{
public:
    CameraController(sp::P<sp::Node> parent)
    : sp::Camera(parent)
    {
        setOrtographic(25.0);
        setRotation(-90);
    }

    virtual void onUpdate(float delta) override
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
            setOrtographic(500.0);
        else
            setOrtographic(25.0);
#endif
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            sp::Engine::getInstance()->shutdown();
    }
};

class TileIndexBuilder
{
public:
    sp::Vector2i index[256];
    
    TileIndexBuilder()
    {
        for(int n=0; n<256; n++)
            index[n] = sp::Vector2i(-1, -1);
    }

    void set(sp::Vector2i tile_index, const char* data)
    {
        int mask = 0;
        int value = 0;
        for(int n=0; n<8; n++)
        {
            int c = n < 4 ? data[n] : data[n+1];
            if (c == '?')
                continue;
            mask |= 1 << n;
            if (c == '#')
                value |= 1 << n;
        }
        for(int n=0; n<256; n++)
        {
            if ((n & mask) == value)
            {
                index[n] = tile_index;
            }
        }
    }
};

int main(int argc, char** argv)
{
    //Create resource providers, so we can load things.
    new sp::io::DirectoryResourceProvider("resources");
    sp::textureManager.setDefaultSmoothFiltering(false);
    
    //Load our ui theme.
    sp::gui::Theme::loadTheme("default", "gui/theme/basic.theme.txt");
    
    //Create a window to render on, and our engine.
    sp::P<sp::Window> window = new sp::Window(4.0/3.0);
    sp::P<sp::Engine> engine = new sp::Engine();
    
    //Initialize the player keys, needs to be done before keybindings are loaded.
    PlayerKeys::init();
    WeaponInfo::init();
    
    {
        scene = new sp::Scene("main_scene");
    }
    
    sp::P<ShadowRenderPass> shadow_pass;
    {
        gui_scene = new sp::gui::Scene(sp::Vector2d(1280, 800), sp::gui::Scene::Direction::Horizontal);

        camera = new CameraController(scene->getRoot());

        scene_layer = new sp::SceneGraphicsLayer(10);
        shadow_pass = new ShadowRenderPass(scene, camera);
        scene_layer->addRenderPass(shadow_pass);
#ifdef DEBUG
        scene_layer->addRenderPass(new sp::CollisionRenderPass(scene, camera));
#endif
        scene_layer->addRenderPass(new sp::BasicNodeRenderPass(gui_scene->getCamera()));
        window->addLayer(scene_layer);
    }
    
    {
        new HudManager(scene->getRoot());
    }
    if (0)
    {
        new Editor(gui_scene->getRoot(), "prefab/special/start");
    }
    else
    {
        Player* p = new Player(0);
        shadow_pass->light_sources.add(p);
        
        if (0)
        {
            p = new Player(1);
            shadow_pass->light_sources.add(p);
            p->setPosition(sp::Vector2d(0.1, 0));
            p->setRotation(180);
        }
        new WeaponPickup();
        //MapGenerator mg;
        //mg.generate();
        
        //new DynamicSectorLoader();
        
        for(int n=0;n<100;n++)
            (new Enemy())->setPosition(sp::Vector2d(sp::random(4, 5), -5));
        
        sp::Tilemap* tm = new sp::Tilemap(scene->getRoot(), "tilesheet_transparent.png", 2.0, 2.0, 34, 20);
        tm->render_data.order = -10;
        tm->setRotation(15);
        TileIndexBuilder tib;
        sp::io::ResourceStreamPtr tile_map = sp::io::ResourceProvider::get("tile_map.txt");
        
        for(unsigned int m=0; m<6; m++)
        {
            sp::string line0 = tile_map->readLine();
            sp::string line1 = tile_map->readLine();
            sp::string line2 = tile_map->readLine();
            tile_map->readLine();
            for(unsigned int n=0; n<line0.length(); n+=4)
            {
                char buffer[9] = {
                    line0[n], line0[n+1], line0[n+2],
                    line1[n], line1[n+1], line1[n+2],
                    line2[n], line2[n+1], line2[n+2],
                };
                tib.set(sp::Vector2i(n/4, m), buffer);
            }
        }
        
        for(int n=0; n<256; n++)
        {
            int x = (n % 16) * 3;
            int y = (n / 16) * 3;
            tm->setTile(x + 0, y + 2, sp::random(0, 100) < 20 ? 1 : 0);
            tm->setTile(x + 1, y + 2, sp::random(0, 100) < 20 ? 1 : 0);
            tm->setTile(x + 2, y + 2, sp::random(0, 100) < 20 ? 1 : 0);

            tm->setTile(x + 0, y + 1, sp::random(0, 100) < 20 ? 1 : 0);
            tm->setTile(x + 2, y + 1, sp::random(0, 100) < 20 ? 1 : 0);

            tm->setTile(x + 0, y + 0, sp::random(0, 100) < 20 ? 1 : 0);
            tm->setTile(x + 1, y + 0, sp::random(0, 100) < 20 ? 1 : 0);
            tm->setTile(x + 2, y + 0, sp::random(0, 100) < 20 ? 1 : 0);
            
            tm->setTile(x + 1, y + 1, sp::random(0, 100) < 50 ? 1 : 0);
        }
        
        for(int x=0; x<16*3; x++)
        {
            for(int y=0; y<16*3; y++)
            {
                int n = 0;
                if (tm->getTileIndex(x, y) < 1) continue;
                
                if (tm->getTileIndex(x - 1, y - 1) > 0) n |= 1 << 5;
                if (tm->getTileIndex(x + 0, y - 1) > 0) n |= 1 << 6;
                if (tm->getTileIndex(x + 1, y - 1) > 0) n |= 1 << 7;

                if (tm->getTileIndex(x - 1, y + 0) > 0) n |= 1 << 3;
                if (tm->getTileIndex(x + 1, y + 0) > 0) n |= 1 << 4;

                if (tm->getTileIndex(x - 1, y + 1) > 0) n |= 1 << 0;
                if (tm->getTileIndex(x + 0, y + 1) > 0) n |= 1 << 1;
                if (tm->getTileIndex(x + 1, y + 1) > 0) n |= 1 << 2;
                
                tm->setTile(x, y, tib.index[n].x + (tib.index[n].y + 4) * 34, sp::Tilemap::Collision::Solid);
            }
        }
    }
    engine->run();
    
    return 0;
}

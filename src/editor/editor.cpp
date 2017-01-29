#include "editor.h"
#include "editorHandle.h"
#include "prototype.h"
#include "../main.h"

#include <sp2/graphics/gui/guiLoader.h>
#include <sp2/engine.h>

Editor::Editor(sp::P<sp::gui::Widget> parent)
: sp::gui::Widget(parent)
{
    layout.fill_width = true;
    layout.fill_height = true;
    selection_layer = Prefab::Part::Type::Wall;
    
    sp::P<sp::gui::Widget> editor_ui = sp::gui::Loader::load("gui/editor.gui", "EDITOR", this);
    editor_ui->getWidgetWithID("CREATE")->setEventCallback([this](sp::Variant v)
    {
        new Prototype(selection_layer, sp::Vector2d(0, 0), 0, sp::Vector2f(5, 5));
    });
    editor_ui->getWidgetWithID("LAYER_FLOOR")->setEventCallback([this](sp::Variant v)
    {
        selection_layer = Prefab::Part::Type::Floor;
    });
    editor_ui->getWidgetWithID("LAYER_WALL")->setEventCallback([this](sp::Variant v)
    {
        selection_layer = Prefab::Part::Type::Wall;
    });
    editor_ui->getWidgetWithID("LAYER_DOOR")->setEventCallback([this](sp::Variant v)
    {
        selection_layer = Prefab::Part::Type::Door;
    });
    editor_ui->getWidgetWithID("LAYER_TRIGGER")->setEventCallback([this](sp::Variant v)
    {
        selection_layer = Prefab::Part::Type::Trigger;
    });
    editor_ui->getWidgetWithID("LAYER_PREFAB_CONNECTION")->setEventCallback([this](sp::Variant v)
    {
        selection_layer = Prefab::Part::Type::PrefabConnection;
    });
    editor_ui->getWidgetWithID("QUIT")->setEventCallback([this](sp::Variant v)
    {
        savePrefab("resources/prefab/small_hallway_t.prefab");
        sp::Engine::getInstance()->shutdown();
    });

    editor_ui->getWidgetWithID("COLOR_H")->setEventCallback([this](sp::Variant v)
    {
        if (selection)
        {
            selection->color.hue = v.getDouble();
            selection->render_data.color = selection->color;
        }
    });
    editor_ui->getWidgetWithID("COLOR_S")->setEventCallback([this](sp::Variant v)
    {
        if (selection)
        {
            selection->color.saturation = v.getDouble();
            selection->render_data.color = selection->color;
        }
    });
    editor_ui->getWidgetWithID("COLOR_V")->setEventCallback([this](sp::Variant v)
    {
        if (selection)
        {
            selection->color.value = v.getDouble();
            selection->render_data.color = selection->color;
        }
    });
    
    Prefab prefab;
    prefab.load("resources/prefab/small_hallway_t.prefab");
    prefab.createPrototypes();
}

bool Editor::onPointerDown(sp::io::Pointer::Button button, sp::Vector2f position, int id)
{
    sp::Vector2d world_position = toWorld(position);

    if (selection)
    {
        handle = selection->getHandle(world_position);
        if (handle)
            return true;
        selection->destroyHandles();
    }
    selection = nullptr;
    
    scene->queryCollision(world_position, [this](sp::P<sp::SceneNode> node)
    {
        sp::P<Prototype> prototype = node;
        if (prototype && prototype->type == selection_layer)
        {
            selection = prototype;
            selection->createHandles();
            getWidgetWithID("COLOR_H")->setAttribute("value", selection->color.hue);
            getWidgetWithID("COLOR_S")->setAttribute("value", selection->color.saturation);
            getWidgetWithID("COLOR_V")->setAttribute("value", selection->color.value);
            return false;
        }
        return true;
    });
    pointer_position = position;
    return true;
}

void Editor::onPointerDrag(sf::Vector2f position, int id)
{
    sp::Vector2d world_position = toWorld(position);
    
    if (handle)
    {
        handle->dragTo(world_position);
        return;
    }
    if (selection)
    {
        sp::Vector2d previous_world_position = toWorld(pointer_position);
        
        selection->setPosition(selection->getGlobalPosition2D() - previous_world_position + world_position);
    }
    pointer_position = position;
}

void Editor::onPointerUp(sf::Vector2f position, int id)
{
}

void Editor::savePrefab(sp::string filename)
{
    Prefab prefab;
    prefab.updateFromPrototypes();
    prefab.save(filename);
}

void Editor::loadPrefab(sp::string filename)
{
    
}

sp::Vector2d Editor::toWorld(sp::Vector2f position)
{
    sp::Vector2d screen_position = sp::Vector2d(gui_layer->virtualPositionToScreen(position)) * 2.0 - sp::Vector2d(1, 1);
    screen_position.y = -screen_position.y;
    sp::Vector2d world_position = camera->getProjectionMatrix().inverse() * screen_position;
    return camera->getLocalPoint2D(world_position);
}

#include "editor.h"
#include "editorHandle.h"
#include "prototype.h"
#include "../main.h"

#include <SFML/Window/Keyboard.hpp>
#include <sp2/graphics/gui/loader.h>
#include <sp2/engine.h>

Editor::Editor(sp::P<sp::gui::Widget> parent, sp::string prefab_name)
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
    editor_ui->getWidgetWithID("LAYER_SPAWNER")->setEventCallback([this](sp::Variant v)
    {
        selection_layer = Prefab::Part::Type::Spawner;
    });
    editor_ui->getWidgetWithID("QUIT")->setEventCallback([this, prefab_name](sp::Variant v)
    {
        savePrefab(prefab_name);
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
    prefab.load(prefab_name);
    prefab.createPrototypes();
}
/*
void Editor::render(sf::RenderTarget& window)
{
    Widget::render(window);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Delete) && selection)
    {
        LOG(Debug, "DEL");
        delete *selection;
    }
}
*/
bool Editor::onPointerDown(sp::io::Pointer::Button button, sp::Vector2d position, int id)
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
    
    getScene()->queryCollision(world_position, [this](sp::P<sp::Node> node)
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

void Editor::onPointerDrag(sp::Vector2d position, int id)
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

void Editor::onPointerUp(sp::Vector2d position, int id)
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

sp::Vector2d Editor::toWorld(sp::Vector2d position)
{
    //return camera->screenToWorld(gui_scene->virtualPositionToScreen(position));
}

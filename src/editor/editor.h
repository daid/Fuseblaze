#ifndef EDITOR_EDITOR_H
#define EDITOR_EDITOR_H

#include <sp2/graphics/gui/widget/widget.h>
#include "../prefab.h"

class Prototype;
class EditorHandle;

class Editor : public sp::gui::Widget
{
public:
    Editor(sp::P<sp::gui::Widget> parent, sp::string prefab_name);
        
    //virtual void render(sf::RenderTarget& window) override;
    virtual bool onPointerDown(sp::io::Pointer::Button button, sp::Vector2d position, int id) override;
    
    virtual void onPointerDrag(sp::Vector2d position, int id) override;
    
    virtual void onPointerUp(sp::Vector2d position, int id) override;
    void savePrefab(sp::string filename);
    
    void loadPrefab(sp::string filename);

private:
    sp::P<Prototype> selection;
    sp::P<EditorHandle> handle;
    sp::Vector2d pointer_position;
    
    Prefab::Part::Type selection_layer;
    
    sp::Vector2d toWorld(sp::Vector2d position);
};

#endif//EDITOR_EDITOR_H

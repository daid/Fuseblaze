#ifndef EDITOR_EDITOR_H
#define EDITOR_EDITOR_H

#include <sp2/graphics/gui/widget/widget.h>
#include "../prefab.h"

class Prototype;
class EditorHandle;

class Editor : public sp::gui::Widget
{
public:
    Editor(sp::P<sp::gui::Widget> parent);
        
    virtual bool onPointerDown(sp::io::Pointer::Button button, sp::Vector2f position, int id) override;
    
    virtual void onPointerDrag(sf::Vector2f position, int id) override;
    
    virtual void onPointerUp(sf::Vector2f position, int id) override;
    void savePrefab(sp::string filename);
    
    void loadPrefab(sp::string filename);

private:
    sp::P<Prototype> selection;
    sp::P<EditorHandle> handle;
    sp::Vector2f pointer_position;
    
    Prefab::Part::Type selection_layer;
    
    sp::Vector2d toWorld(sp::Vector2f position);
};

#endif//EDITOR_EDITOR_H

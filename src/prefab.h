#ifndef PREFAB_H
#define PREFAB_H

#include <sp2/math/vector.h>
#include <sp2/graphics/color.h>
#include <sp2/scene/node.h>
#include <vector>

class Prefab
{
public:
    class Part
    {
    public:
        enum Type
        {
            Floor,
            Wall,
            Door,
            Trigger,
            PrefabConnection
        };
        
        Type type;
        sp::Vector2d position;
        double rotation;
        sp::Vector2f size;
        sp::HsvColor color;
        
        sp::string id;
    };
    
    void load(sp::string filename);
    void save(sp::string filename);
    
    std::map<sp::string, sp::P<sp::SceneNode>> spawn(sp::Vector2d position, double rotation);
    std::vector<Part> getConnections();
    void createPrototypes();
    void updateFromPrototypes();
private:
    std::vector<Part> parts;
};

#endif//PREFAB_H

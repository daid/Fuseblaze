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
            PrefabConnection,
            Spawner
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
    std::vector<Part> getParts(Part::Type type);
    void createPrototypes();
    void updateFromPrototypes();
    
    sp::string name;
    
    bool collision(sp::Vector2d position, double rotation, Prefab& other, sp::Vector2d other_position, double other_rotation);
private:
    std::pair<double, double> projectPolygon(sp::Vector2d position, double rotation, sp::Vector2d normal);
    void updateConvexHull();

    std::vector<Part> parts;
    std::vector<sp::Vector2d> convex_hull;
};

#endif//PREFAB_H

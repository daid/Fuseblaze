#ifndef MAP_GENERATOR_H
#define MAP_GENERATOR_H

#include "prefab.h"

class MapGenerator
{
public:
    MapGenerator();
    
    void generate();
    
    sp::Vector2d getPositionForPrefab(sp::Vector2d entrance_position, double entrance_rotation, Prefab& prefab, Prefab::Part& entrance_part);
private:
    std::vector<Prefab> room_prefabs;
    std::vector<Prefab> connector_prefabs;

    class PlacedPrefabs
    {
    public:
        sp::Vector2d position;
        double rotation;
        Prefab& prefab;
        
        PlacedPrefabs(sp::Vector2d position, double rotation, Prefab& prefab)
        : position(position), rotation(rotation), prefab(prefab)
        {
        }
    };
    std::vector<PlacedPrefabs> placed_prefabs;
    int open_ends;

    bool expandMap(int depth, sp::Vector2d position, double rotation, Prefab& prefab, int entrance_index);
};

#endif//MAP_GENERATOR_H

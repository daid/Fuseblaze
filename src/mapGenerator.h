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

    void expandMap(int depth, sp::Vector2d position, double rotation, Prefab& prefab, int entrance_index);
};

#endif//MAP_GENERATOR_H

#include "mapGenerator.h"
#include <sp2/random.h>

MapGenerator::MapGenerator()
{
}

void MapGenerator::generate()
{
    sp::Vector2d spawn_position(0, 0);
    double spawn_rotation = 45;

    Prefab prefab;
    prefab.load("resources/prefab/small_hallway_t.prefab");
    expandMap(0, spawn_position, spawn_rotation, prefab, -1);
}

void MapGenerator::expandMap(int depth, sp::Vector2d spawn_position, double spawn_rotation, Prefab& prefab, int entrance_index)
{
    if (depth > 5)
        return;
    
    sp::Quaterniond q = sp::Quaterniond::fromAngle(spawn_rotation);
    LOG(Debug, spawn_position);
    prefab.spawn(spawn_position, spawn_rotation);
    auto connections = prefab.getConnections();
    for(int n=0; n<int(connections.size()); n++)
    {
        if (n == entrance_index)
            continue;
        auto& part = connections[n];
        
        sp::Vector2d position = spawn_position + q * part.position;
        double rotation = spawn_rotation + part.rotation;
        
        Prefab door_prefab;
        door_prefab.load("resources/prefab/small_door.prefab");
        door_prefab.spawn(position, rotation);
        
        Prefab prefab2;
        if (sp::random(0, 100) < 20)
            prefab2.load("resources/prefab/small_hallway_t.prefab");
        else
            prefab2.load("resources/prefab/small_hallway_corner.prefab");
        auto parts = prefab2.getConnections();
        int index = sp::irandom(0, parts.size() - 1);

        expandMap(depth + 1, getPositionForPrefab(position, rotation, prefab, parts[index]), rotation + 180 - parts[index].rotation, prefab2, index);
    }
}

sp::Vector2d MapGenerator::getPositionForPrefab(sp::Vector2d entrance_position, double entrance_rotation, Prefab& prefab, Prefab::Part& entrance_part)
{
    sp::Quaterniond q = sp::Quaterniond::fromAngle(entrance_rotation - entrance_part.rotation);
    return entrance_position + q * entrance_part.position;
}

#include "mapGenerator.h"
#include "wall.h"
#include "enemy.h"
#include <sp2/random.h>

MapGenerator::MapGenerator()
{
    room_prefabs.resize(4);
    room_prefabs[0].load("prefab/small_hallway_t.prefab");
    room_prefabs[1].load("prefab/small_hallway_corner.prefab");
    room_prefabs[2].load("prefab/small_hallway.prefab");
    room_prefabs[3].load("prefab/small_room1.prefab");
    connector_prefabs.resize(2);
    connector_prefabs[0].load("prefab/small_door.prefab");
    connector_prefabs[1].load("prefab/small_opening.prefab");
}

void MapGenerator::generate()
{
    sp::Vector2d spawn_position(0, 0);
    double spawn_rotation = 45;

    expandMap(0, spawn_position, spawn_rotation, room_prefabs[0], -1);
    
    for(auto& pp : placed_prefabs)
    {
        for(auto& part : pp.prefab.getParts(Prefab::Part::Type::Spawner))
        {
            sp::Vector2d position = pp.position + sp::Quaterniond::fromAngle(pp.rotation) * part.position;
            
            int enemy_count = part.size.x * part.size.y;
            for(int n=0; n<enemy_count; n++)
                (new Enemy())->setPosition(position + sp::Vector2d(sp::random(-part.size.x / 2.0, part.size.x / 2.0), sp::random(-part.size.y / 2.0, part.size.y / 2.0)));
        }
    }
}

bool MapGenerator::expandMap(int depth, sp::Vector2d spawn_position, double spawn_rotation, Prefab& prefab, int entrance_index)
{
    if (depth > 5)
        return false;
    for(auto& pp : placed_prefabs)
    {
        if (prefab.collision(spawn_position, spawn_rotation, pp.prefab, pp.position, pp.rotation))
        {
            LOG(Debug, spawn_position, spawn_rotation, prefab.name);
            return false;
        }
    }
    
    placed_prefabs.emplace_back(spawn_position, spawn_rotation, prefab);

    sp::Quaterniond q = sp::Quaterniond::fromAngle(spawn_rotation);
    prefab.spawn(spawn_position, spawn_rotation);
    auto connections = prefab.getParts(Prefab::Part::Type::PrefabConnection);
    for(int n=0; n<int(connections.size()); n++)
    {
        if (n == entrance_index)
            continue;
        auto& part = connections[n];

        sp::Vector2d position = spawn_position + q * part.position;
        double rotation = spawn_rotation + part.rotation;

        Prefab& prefab2 = room_prefabs[sp::irandom(0, room_prefabs.size() - 1)];
        auto parts = prefab2.getParts(Prefab::Part::Type::PrefabConnection);
        int index = sp::irandom(0, parts.size() - 1);

        if (expandMap(depth + 1, getPositionForPrefab(position, rotation, prefab, parts[index]), rotation + 180 - parts[index].rotation, prefab2, index))
        {
            connector_prefabs[sp::irandom(0, connector_prefabs.size() - 1)].spawn(position, rotation);
        }
        else
        {
            new Wall(position, rotation, part.size, part.color);
        }
    }
    return true;
}

sp::Vector2d MapGenerator::getPositionForPrefab(sp::Vector2d entrance_position, double entrance_rotation, Prefab& prefab, Prefab::Part& entrance_part)
{
    sp::Quaterniond q = sp::Quaterniond::fromAngle(entrance_rotation - entrance_part.rotation);
    return entrance_position + q * entrance_part.position;
}

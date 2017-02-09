#include "mapGenerator.h"
#include "wall.h"
#include "enemy.h"
#include <sp2/random.h>

MapGenerator::MapGenerator()
{
    room_prefabs.resize(5);
    room_prefabs[0].load("prefab/small_hallway_t");
    room_prefabs[1].load("prefab/small_hallway_corner");
    room_prefabs[2].load("prefab/small_hallway");
    room_prefabs[3].load("prefab/small_room1");
    room_prefabs[4].load("prefab/small_hallway_slow_door");
    connector_prefabs.resize(2);
    connector_prefabs[0].load("prefab/small_door");
    connector_prefabs[1].load("prefab/small_opening");
}

void MapGenerator::generate()
{
    sp::Vector2d spawn_position(0, 0);
    double spawn_rotation = 45;

    open_ends = 0;
    expandMap(0, spawn_position, spawn_rotation, room_prefabs[3], -1);
    
    for(auto& pp : placed_prefabs)
    {
        if (&pp == &placed_prefabs[0])
            continue;
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
    if (placed_prefabs.size() > 25)
        return false;
    if (open_ends > 1 && sp::random(0, 100) < 20)
        return false;
    for(auto& pp : placed_prefabs)
    {
        if (prefab.collision(spawn_position, spawn_rotation, pp.prefab, pp.position, pp.rotation))
        {
            return false;
        }
    }
    
    placed_prefabs.emplace_back(spawn_position, spawn_rotation, prefab);

    sp::Quaterniond q = sp::Quaterniond::fromAngle(spawn_rotation);
    prefab.spawn(spawn_position, spawn_rotation);
    auto connections = prefab.getParts(Prefab::Part::Type::PrefabConnection);
    open_ends += connections.size();
    if (entrance_index >= 0)
        open_ends--;
    for(int n=0; n<int(connections.size()); n++)
    {
        if (n == entrance_index)
            continue;
        open_ends--;
        auto& part = connections[n];

        sp::Vector2d position = spawn_position + q * part.position;
        double rotation = spawn_rotation + part.rotation;

        Prefab& prefab2 = room_prefabs[sp::irandom(0, room_prefabs.size() - 1)];
        auto parts = prefab2.getParts(Prefab::Part::Type::PrefabConnection);
        int index = sp::irandom(0, parts.size() - 1);

        if (expandMap(depth + 1, getPositionForPrefab(position, rotation, prefab, parts[index]), rotation + 180 - parts[index].rotation, prefab2, index))
        {
            if (depth == 0)
                connector_prefabs[0].spawn(position, rotation);
            else
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

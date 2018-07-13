#include "dynamicSectorLoader.h"
#include "sectorLoader.h"
#include "player.h"
#include "main.h"

DynamicSectorLoader::DynamicSectorLoader()
: sp::Node(::scene->getRoot())
{
}

void DynamicSectorLoader::onUpdate(float delta)
{
    for(auto player : Player::players)
    {
        sp::Vector2i sector_min = SectorLoader::positionToSector(player->getGlobalPosition2D() + sp::Vector2d(-load_distance, load_distance));
        sp::Vector2i sector_max = SectorLoader::positionToSector(player->getGlobalPosition2D() + sp::Vector2d( load_distance,-load_distance));
        
        for(int level=1; level<6; level++)
        {
            int sector_size = level*level;
            for(int x=sector_min.x-sector_size; x<sector_max.x+sector_size; x++)
                for(int y=sector_min.y-sector_size; y<sector_max.y+sector_size; y++)
                    checkIfNeededToLoad(x, y, level);
        }
    }
    
    std::vector<sp::Vector3i> delete_list;
    for(auto it : loaded_sectors)
    {
        sp::Vector3i v = it.first;
        
        bool keep = false;
        for(auto player : Player::players)
        {
            sp::Vector2i sector_min = SectorLoader::positionToSector(player->getGlobalPosition2D() + sp::Vector2d(-load_distance, load_distance));
            sp::Vector2i sector_max = SectorLoader::positionToSector(player->getGlobalPosition2D() + sp::Vector2d( load_distance,-load_distance));
            if (sector_min.x <= v.x + v.z*v.z && sector_max.x >= v.x - v.z*v.z && sector_min.y <= v.y + v.z*v.z && sector_max.y >= v.y - v.z*v.z)
            {
                keep = true;
                continue;
            }
        }
        if (!keep)
        {
            LOG(Debug, "Deleting:", v);
            delete *it.second;
            
            delete_list.push_back(it.first);
        }
    }
    for(auto v : delete_list)
        loaded_sectors.erase(v);
}

void DynamicSectorLoader::checkIfNeededToLoad(int x, int y, int level)
{
    sp::Vector3i v(x, y, level);
    
    if (loaded_sectors.find(v) != loaded_sectors.end())
        return;
    
    sp::Node* node = new sp::Node(getParent());
    SectorLoader::loadSector(x, y, level, node);
    loaded_sectors[v] = node;
}

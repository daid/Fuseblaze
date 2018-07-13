#ifndef SECTOR_LOADER_H
#define SECTOR_LOADER_H

#include "sp2/scene/node.h"

class SectorLoader
{
public:
    static constexpr float sector_import_scale = 300;
    static constexpr float sector_size = 40000;

    static bool loadSector(int x, int y, int level, sp::P<sp::Node> root);
    static sp::Vector2i positionToSector(const sp::Vector2d position);

private:
    static sp::Node* createSolidNode(sp::P<sp::Node> root, const std::vector<sp::Vector2f>& path);
    static sp::Node* createLineNode(sp::P<sp::Node> root, const std::vector<sp::Vector2f>& path, float width);
    
    static constexpr int sector_offset_x = 812;
    static constexpr int sector_offset_y = 12970;
};

#endif//SECTOR_LOADER_H

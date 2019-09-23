#ifndef DYNAMIC_SECTOR_LOADER_H
#define DYNAMIC_SECTOR_LOADER_H

#include "sp2/scene/node.h"
#include <unordered_map>

namespace std {
template <> struct hash<sp::Vector3i>
{
    size_t operator()(const sp::Vector3i& v) const
    {
        return hash<int>()(v.x ^ v.y ^ v.z);
    }
};
}

class DynamicSectorLoader : public sp::Node
{
public:
    DynamicSectorLoader();
    
    virtual void onUpdate(float delta) override;

private:
    static constexpr float load_distance = 30.0;
    
    void checkIfNeededToLoad(int x, int y, int level);

    std::unordered_map<sp::Vector3i, sp::P<sp::Node>> loaded_sectors;
};

#endif//DYNAMIC_SECTOR_LOADER_H

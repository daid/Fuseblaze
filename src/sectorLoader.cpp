#include <sp2/string.h>
#include <sp2/math/vector.h>
#include <sp2/scene/node.h>
#include <sp2/graphics/meshdata.h>
#include <sp2/collision/2d/chains.h>

#include <winsock.h>

#include "sectorLoader.h"
#include "math/triangulate.h"

bool SectorLoader::loadSector(int x, int y, int level, sp::P<sp::Node> root)
{
    if (level == 1)
    {
        int px = x - sector_offset_x;
        int py = y - sector_offset_y;
        float s = sector_size / sector_import_scale * level * level;
        float s0 = 10 - level * 3;
        float s1 = s - (10 - level * 3);

        sp::Vector2f v0(float(py) * sector_size / sector_import_scale, float(-px) * sector_size / sector_import_scale);
        std::vector<sp::Vector2f> path;
        path.emplace_back(v0 + sp::Vector2f(s0, -s0));
        path.emplace_back(v0 + sp::Vector2f(s1, -s0));
        path.emplace_back(v0 + sp::Vector2f(s1, -s1));
        path.emplace_back(v0 + sp::Vector2f(s0, -s1));
        path.emplace_back(v0 + sp::Vector2f(s0, -s0));
        sp::Node* node = createLineNode(root, path, 2);
        node->render_data.color = sp::HsvColor(0, 100, 50);
        node->render_data.order = -5;
    }

    sp::string filename = "_output/" + sp::string(level) + "." + sp::string(x) + "." + sp::string(y) + ".data";
    FILE* f = fopen(filename.c_str(), "rb");
    if (!f)
    {
        //LOG(Warning, "Cannot load sector", x, y, level);
        return false;
    }
    LOG(Info, "Loading sector", x, y, level);
    sp::MeshData::Vertices building_vertices;
    sp::MeshData::Indices building_indices;
    sp::collision::Chains2D building_collision;

    while(!feof(f))
    {
        uint32_t type_id;
        if (fread(&type_id, 1, sizeof(type_id), f) < 1)
            break;
        type_id = htonl(type_id);
        uint32_t path_count;
        fread(&path_count, 1, sizeof(path_count), f);
        path_count = htonl(path_count);
        for(unsigned int n=0; n<path_count; n++)
        {
            uint32_t point_count;
            fread(&point_count, 1, sizeof(point_count), f);
            point_count = htonl(point_count);
            std::vector<sp::Vector2f> path;
            for(unsigned int index=0; index<point_count; index++)
            {
                int32_t x, y;
                fread(&x, 1, sizeof(x), f);
                x = htonl(x);
                fread(&y, 1, sizeof(y), f);
                y = htonl(y);
                
                x -= sector_offset_x * sector_size;
                y -= sector_offset_y * sector_size;
                
                sp::Vector2f v(double(y) / sector_import_scale, double(-x) / sector_import_scale);
                if (path.size() == 0 || (path.back() - v).length() > 0.010f)
                    path.emplace_back(v);
            }
            
            if (type_id == 0x00010000)
            {
                createSolidNode(root, path)->render_data.color = sp::Color(0,0,1);
                building_collision.loops.emplace_back(path);
            }
            else if (type_id == 0x00010001)
                createSolidNode(root, path)->render_data.color = sp::Color(0,1,0);
            else if (type_id == 0x00020000)
            {
                std::vector<sp::Vector2f> triangles;
                if (Triangulate<float>::process(path, triangles))
                {
                    building_collision.loops.emplace_back(path);
                    
                    for(auto p : triangles)
                    {
                        building_vertices.emplace_back(sp::Vector3f(p.x, p.y, 0.0));
                        building_indices.push_back(building_indices.size());
                    }
                }
            }
            else if (type_id == 0x00030000)
            {
                sp::Node* node = createLineNode(root, path, 1);
                node->render_data.color = sp::HsvColor(0, 0, 50);
                node->render_data.order = -5;
            }
        }
        uint32_t property_count;
        fread(&property_count, 1, sizeof(property_count), f);
        for(unsigned int n=0; n<property_count; n++)
        {
            uint8_t size;
            char buffer[256];

            fread(&size, 1, sizeof(size), f);
            fread(buffer, 1, size, f);
            buffer[size] = 0;

            fread(&size, 1, sizeof(size), f);
            fread(buffer, 1, size, f);
            buffer[size] = 0;
        }
    }
    fclose(f);
    
    if (building_collision.loops.size() > 0)
    {
        sp::Node* node = new sp::Node(root);
        if (building_vertices.size() > 0)
        {
            node->render_data.type = sp::RenderData::Type::Normal;
            node->render_data.shader = sp::Shader::get("internal:color.shader");
            node->render_data.mesh = std::make_shared<sp::MeshData>(std::move(building_vertices), std::move(building_indices));
        }
        root->setCollisionShape(building_collision);
    }
    
    return true;
}

sp::Vector2i SectorLoader::positionToSector(const sp::Vector2d position)
{
    sp::Vector2i sector(-position.y * sector_import_scale / sector_size, position.x * sector_import_scale / sector_size);
    sector.x += sector_offset_x;
    sector.y += sector_offset_y;
    return sector;
}

sp::Node* SectorLoader::createSolidNode(sp::P<sp::Node> root, const std::vector<sp::Vector2f>& path)
{
    std::vector<sp::Vector2f> triangles;
    if (Triangulate<float>::process(path, triangles))
    {
        sp::MeshData::Vertices vertices;
        sp::MeshData::Indices indices;
        for(auto p : triangles)
        {
            vertices.emplace_back(sp::Vector3f(p.x, p.y, 0.0));
            indices.push_back(indices.size());
        }
        sp::Node* node = new sp::Node(root);
        node->render_data.type = sp::RenderData::Type::Normal;
        node->render_data.shader = sp::Shader::get("internal:color.shader");
        node->render_data.mesh = std::make_shared<sp::MeshData>(std::move(vertices), std::move(indices));
        return node;
    }
    return nullptr;
}

sp::Node* SectorLoader::createLineNode(sp::P<sp::Node> root, const std::vector<sp::Vector2f>& path, float width)
{
    std::vector<sp::Vector2f> normals;
    sp::MeshData::Vertices vertices;
    sp::MeshData::Indices indices;
    
    for(unsigned int idx0=0, idx1=1; idx1<path.size(); idx0=idx1, idx1++)
    {
        sp::Vector2f p0 = path[idx0];
        sp::Vector2f p1 = path[idx1];
        sp::Vector2f direction = (p1 - p0).normalized();
        sp::Vector2f normal(direction.y, -direction.x);
        
        normals.push_back(normal);
    }
        
    for(unsigned int idx0=0, idx1=1; idx1<path.size(); idx0=idx1, idx1++)
    {
        sp::Vector2f p0 = path[idx0];
        sp::Vector2f p1 = path[idx1];
        sp::Vector2f normal0 = normals[idx0];
        sp::Vector2f normal1 = normals[idx0];
        if (idx0 > 0)
            normal0 = (normal0 + normals[idx0 - 1]).normalized();
        if (idx1 < normals.size())
            normal1 = (normal1 + normals[idx1]).normalized();

        sp::Vector2f v0 = p0 + normal0 * width;
        sp::Vector2f v1 = p0 - normal0 * width;
        sp::Vector2f v2 = p1 + normal1 * width;
        sp::Vector2f v3 = p1 - normal1 * width;

        int i = vertices.size();
        vertices.emplace_back(sp::Vector3f(v0.x, v0.y, 0));
        vertices.emplace_back(sp::Vector3f(v1.x, v1.y, 0));
        vertices.emplace_back(sp::Vector3f(v2.x, v2.y, 0));
        vertices.emplace_back(sp::Vector3f(v3.x, v3.y, 0));

        indices.push_back(i + 0);
        indices.push_back(i + 2);
        indices.push_back(i + 3);

        indices.push_back(i + 0);
        indices.push_back(i + 3);
        indices.push_back(i + 1);
    }
    
    sp::Node* node = new sp::Node(root);
    node->render_data.type = sp::RenderData::Type::Normal;
    node->render_data.shader = sp::Shader::get("internal:color.shader");
    node->render_data.mesh = std::make_shared<sp::MeshData>(std::move(vertices), std::move(indices));
    return node;
}

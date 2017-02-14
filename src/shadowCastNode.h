#ifndef SHADOW_CAST_NODE_H
#define SHADOW_CAST_NODE_H

#include "main.h"
#include <sp2/graphics/meshdata.h>

class ShadowCastNode : public sp::SceneNode
{
public:
    ShadowCastNode(sp::P<sp::SceneNode> parent, sp::Vector2f size)
    : sp::SceneNode(parent)
    {
        //Do not render per default, we use our custom render pass.
        render_data.type = sp::RenderData::Type::None;
        
        std::vector<sp::Vector2f> points;
        points.emplace_back(-size.x * 0.5f, -size.y * 0.5f);
        if (size.x > 5.0)
            points.emplace_back(0, -size.y * 0.5f);
        points.emplace_back( size.x * 0.5f, -size.y * 0.5f);
        if (size.y > 5.0)
            points.emplace_back( size.x * 0.5f,  0);
        points.emplace_back( size.x * 0.5f,  size.y * 0.5f);
        if (size.x > 5.0)
            points.emplace_back(0, size.y * 0.5f);
        points.emplace_back(-size.x * 0.5f,  size.y * 0.5f);
        if (size.y > 5.0)
            points.emplace_back(-size.x * 0.5f,  0);
        
        std::vector<sp::MeshData::Vertex> vertices;
        for(unsigned int n=0; n<points.size(); n++)
        {
            sp::Vector2f p0 = points[n];
            sp::Vector2f p1 = points[(n + 1) % points.size()];
            vertices.emplace_back(sp::Vector3f(p0.x, p0.y, min_shadow_distance));
            vertices.emplace_back(sp::Vector3f(p1.x, p1.y, min_shadow_distance));
            vertices.emplace_back(sp::Vector3f(p0.x, p0.y, max_shadow_distance));

            vertices.emplace_back(sp::Vector3f(p0.x, p0.y, max_shadow_distance));
            vertices.emplace_back(sp::Vector3f(p1.x, p1.y, min_shadow_distance));
            vertices.emplace_back(sp::Vector3f(p1.x, p1.y, max_shadow_distance));
        }
        render_data.mesh = std::make_shared<sp::MeshData>(vertices);
        render_data.shader = sp::Shader::get("shader/wallShadow.shader");
    }

    ShadowCastNode(sp::P<sp::SceneNode> parent, int index, float radius)
    : sp::SceneNode(parent)
    {
        //Do not render per default, we use our custom render pass.
        render_data.type = sp::RenderData::Type::None;
        
        std::vector<sp::Vector2f> points;
        int point_count = 32;
        for(int n=0; n<point_count; n++)
        {
            float a = (float(n) / float(point_count-1)) * (sp::pi * 2.0);
            points.emplace_back(sin(a) * radius, cos(a) * radius);
        }

        std::vector<sp::MeshData::Vertex> vertices;
        for(unsigned int n=0; n<points.size() - 1; n++)
        {
            sp::Vector2f p0 = points[n];
            sp::Vector2f p1 = points[(n + 1) % points.size()];
            vertices.emplace_back(sp::Vector3f(p0.x, p0.y, min_shadow_distance));
            vertices.emplace_back(sp::Vector3f(p1.x, p1.y, min_shadow_distance));
            vertices.emplace_back(sp::Vector3f(p0.x, p0.y, max_shadow_distance));

            vertices.emplace_back(sp::Vector3f(p0.x, p0.y, max_shadow_distance));
            vertices.emplace_back(sp::Vector3f(p1.x, p1.y, min_shadow_distance));
            vertices.emplace_back(sp::Vector3f(p1.x, p1.y, max_shadow_distance));
        }
        render_data.mesh = std::make_shared<sp::MeshData>(vertices);
        render_data.shader = sp::Shader::get("shader/wallShadow.shader");
        render_data.order = index + 1;
    }

    ShadowCastNode(sp::P<sp::SceneNode> parent, int index, float radius, float view_angle)
    : sp::SceneNode(parent)
    {
        //Do not render per default, we use our custom render pass.
        render_data.type = sp::RenderData::Type::None;
        
        std::vector<sp::Vector2f> points;
        int point_count = 16;
        for(int n=0; n<point_count; n++)
        {
            float rad = view_angle / 180.0 * sp::pi;
            float a = (float(n) / float(point_count-1)) * (sp::pi * 2.0 - rad) + rad * 0.5f + sp::pi * 0.5f;
            points.emplace_back(sin(a) * radius, cos(a) * radius);
        }

        std::vector<sp::MeshData::Vertex> vertices;
        for(unsigned int n=0; n<points.size() - 1; n++)
        {
            sp::Vector2f p0 = points[n];
            sp::Vector2f p1 = points[(n + 1) % points.size()];
            vertices.emplace_back(sp::Vector3f(p0.x, p0.y, min_shadow_distance));
            vertices.emplace_back(sp::Vector3f(p1.x, p1.y, min_shadow_distance));
            vertices.emplace_back(sp::Vector3f(p0.x, p0.y, max_shadow_distance));

            vertices.emplace_back(sp::Vector3f(p0.x, p0.y, max_shadow_distance));
            vertices.emplace_back(sp::Vector3f(p1.x, p1.y, min_shadow_distance));
            vertices.emplace_back(sp::Vector3f(p1.x, p1.y, max_shadow_distance));
        }
        render_data.mesh = std::make_shared<sp::MeshData>(vertices);
        render_data.shader = sp::Shader::get("shader/wallShadow.shader");
        render_data.order = index + 1;
    }
private:
    static constexpr float min_shadow_distance = 0.8;
    static constexpr float max_shadow_distance = 1000.0;
};

#endif//SHADOW_CAST_NODE_H

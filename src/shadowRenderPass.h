#ifndef SHADOW_RENDER_PASS_H
#define SHADOW_RENDER_PASS_H

#include "main.h"

class ShadowRenderPass : public sp::RenderPass
{
public:
    ShadowRenderPass(sp::string target_layer, sp::P<sp::Scene> scene, sp::P<sp::CameraNode> camera)
    : sp::RenderPass(target_layer)
    {
        single_scene = scene;
        specific_camera = camera;
        
        overlay_quad = sp::MeshData::createQuad(sp::Vector2f(2, 2));
    }
    
    virtual void render(sf::RenderTarget& target, sp::P<sp::GraphicsLayer> layer, float aspect_ratio)
    {
        if (single_scene->isEnabled() && specific_camera && light_sources.size() > 0)
        {
            camera->setAspectRatio(aspect_ratio);

            glClear(GL_STENCIL_BUFFER_BIT);
            glEnable(GL_STENCIL_TEST);
            glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
            glDepthMask(GL_FALSE);
            
            int n = 0;
            for(sp::SceneNode* light_source : light_sources)
            {
                glStencilMask(1 << n);
                glStencilFunc(GL_ALWAYS, 1 << n, 1 << n);
                glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);

                queue.clear();
                recursiveNodeRender(*scene->getRoot(), n + 1);

                sp::Shader::get("shader/wallShadow.shader")->setUniform("light_source_position", sp::Vector2f(light_source->getGlobalPosition2D()));
                queue.render(camera->getProjectionMatrix(), camera->getGlobalTransform().inverse(), target);
                
                n++;
            }

            glStencilFunc(GL_EQUAL, (1 << n) - 1, (1 << n) - 1);
            glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
            glDepthMask(GL_TRUE);
            glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

            {
                sp::RenderQueue queue2;

                sp::RenderData render_data;
                render_data.type = sp::RenderData::Type::Normal;
                render_data.shader = sp::Shader::get("shader/overlay.shader");
                render_data.mesh = overlay_quad;
                render_data.color = sp::Color::Black;
                queue2.add(sp::Matrix4x4d::identity(), render_data);

                queue2.render(camera->getProjectionMatrix(), camera->getGlobalTransform().inverse(), target);
            }

            glDisable(GL_STENCIL_TEST);
        }
    }

    void recursiveNodeRender(sp::SceneNode* node, int index)
    {
        if (node->render_data.type == sp::RenderData::Type::None && (node->render_data.order == 0 || node->render_data.order == index) && dynamic_cast<ShadowCastNode*>(node))
        {
            sp::RenderData render_data;
            render_data.type = sp::RenderData::Type::Normal;
            render_data.shader = node->render_data.shader;
            render_data.mesh = node->render_data.mesh;
            render_data.color = sp::Color::White;
            queue.add(node->getGlobalTransform(), render_data);
        }

        for(sp::SceneNode* child : node->getChildren())
        {
            recursiveNodeRender(child, index);
        }
    }

    sp::PList<sp::SceneNode> light_sources;
private:
    sp::P<sp::Scene> single_scene;
    sp::P<sp::CameraNode> specific_camera;
    std::shared_ptr<sp::MeshData> overlay_quad;
    
    sp::RenderQueue queue;
};

#endif//SHADOW_RENDER_PASS_H

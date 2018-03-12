#ifndef SHADOW_RENDER_PASS_H
#define SHADOW_RENDER_PASS_H

#include "main.h"

class ShadowRenderPass : public sp::RenderPass
{
public:
    ShadowRenderPass(sp::string target_layer, sp::P<sp::Scene> scene, sp::P<sp::Camera> camera)
    : sp::RenderPass(target_layer)
    {
        single_scene = scene;
        specific_camera = camera;
        
        overlay_quad = sp::MeshData::createQuad(sp::Vector2f(2, 2));
    }
    
    virtual void render(sf::RenderTarget& target, sp::P<sp::GraphicsLayer> layer, float aspect_ratio)
    {
        if (single_scene->isEnabled() && specific_camera)
        {
            camera->setAspectRatio(aspect_ratio);

            pre_shadow_queue.clear();
            shadow_queue.clear();
            player_shadow_queue[0].clear();
            player_shadow_queue[1].clear();
            post_shadow_queue.clear();

            recursiveNodeRender(*scene->getRoot());

            pre_shadow_queue.render(camera->getProjectionMatrix(), camera->getGlobalTransform().inverse(), target);

            glClear(GL_STENCIL_BUFFER_BIT);
            glEnable(GL_STENCIL_TEST);
            glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
            glDepthMask(GL_FALSE);
            
            int n = 0;
            for(sp::Node* light_source : light_sources)
            {
                glStencilMask(1 << n);
                glStencilFunc(GL_ALWAYS, 1 << n, 1 << n);
                glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);

                sp::Shader::get("shader/wallShadow.shader")->setUniformTmp("light_source_position", sp::Vector2f(light_source->getGlobalPosition2D()));
                shadow_queue.render(camera->getProjectionMatrix(), camera->getGlobalTransform().inverse(), target);
                player_shadow_queue[n].render(camera->getProjectionMatrix(), camera->getGlobalTransform().inverse(), target);
                
                n++;
            }

            glStencilFunc(GL_EQUAL, (1 << n) - 1, (1 << n) - 1);
            glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
            glDepthMask(GL_TRUE);
            glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

            if (light_sources.size() > 0)
            {
                sp::RenderQueue queue2;

                sp::RenderData render_data;
                render_data.type = sp::RenderData::Type::Normal;
                render_data.shader = sp::Shader::get("shader/overlay.shader");
                render_data.mesh = overlay_quad;
                render_data.color = sp::Color(0,0,0);
                queue2.add(sp::Matrix4x4d::identity(), render_data);

                //queue2.render(camera->getProjectionMatrix(), camera->getGlobalTransform().inverse(), target);
            }

            glDisable(GL_STENCIL_TEST);
            
            post_shadow_queue.render(camera->getProjectionMatrix(), camera->getGlobalTransform().inverse(), target);
        }
    }

    void recursiveNodeRender(sp::Node* node)
    {
        if (node->render_data.type == sp::RenderData::Type::None)
        {
            if (dynamic_cast<ShadowCastNode*>(node))
            {
                sp::RenderData render_data;
                render_data.type = sp::RenderData::Type::Normal;
                render_data.shader = node->render_data.shader;
                render_data.mesh = node->render_data.mesh;
                render_data.color = sp::Color(1,1,1);
                if (node->render_data.order == 0)
                    shadow_queue.add(node->getGlobalTransform(), render_data);
                else if (node->render_data.order < 3)
                    player_shadow_queue[node->render_data.order-1].add(node->getGlobalTransform(), render_data);
            }
        }
        else
        {
            if (node->render_data.order < 20)
                pre_shadow_queue.add(node->getGlobalTransform(), node->render_data);
            else
                post_shadow_queue.add(node->getGlobalTransform(), node->render_data);
        }

        for(sp::Node* child : node->getChildren())
        {
            recursiveNodeRender(child);
        }
    }

    sp::PList<sp::Node> light_sources;
private:
    sp::P<sp::Scene> single_scene;
    sp::P<sp::Camera> specific_camera;
    std::shared_ptr<sp::MeshData> overlay_quad;
    
    sp::RenderQueue pre_shadow_queue;
    sp::RenderQueue shadow_queue;
    sp::RenderQueue player_shadow_queue[2];
    sp::RenderQueue post_shadow_queue;
};

#endif//SHADOW_RENDER_PASS_H

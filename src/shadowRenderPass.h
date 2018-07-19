#ifndef SHADOW_RENDER_PASS_H
#define SHADOW_RENDER_PASS_H

#include "main.h"

class ShadowRenderPass : public sp::RenderPass
{
public:
    ShadowRenderPass(sp::P<sp::Scene> scene, sp::P<sp::Camera> camera)
    : sp::RenderPass()
    {
        single_scene = scene;
        specific_camera = camera;
    }
    
    virtual void render(sp::RenderQueue& queue) override
    {
        if (single_scene->isEnabled() && specific_camera)
        {
            queue.setCamera(camera);

            mode = Mode::PreShadow;
            recursiveNodeRender(queue, *scene->getRoot());

            queue.add([]()
            {
                glClear(GL_STENCIL_BUFFER_BIT);
                glEnable(GL_STENCIL_TEST);
                glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
                glDepthMask(GL_FALSE);
            });
            
            int n = 0;
            for(sp::Node* light_source : light_sources)
            {
                sp::Vector2f pos = sp::Vector2f(light_source->getGlobalPosition2D());
                
                queue.add([n, pos]()
                {
                    glStencilMask(1 << n);
                    glStencilFunc(GL_ALWAYS, 1 << n, 1 << n);
                    glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
                    sp::Shader::get("shader/wallShadow.shader")->setUniformTmp("light_source_position", pos);
                });

                mode = Mode::Shadow;
                light_index = n;
                recursiveNodeRender(queue, *scene->getRoot());
                
                n++;
            }
            
            queue.add([n]()
            {
                glStencilFunc(GL_EQUAL, (1 << n) - 1, (1 << n) - 1);
                glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
                glDepthMask(GL_TRUE);
                glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
            });

            if (light_sources.size() > 0)
            {
                sp::RenderData render_data;
                render_data.type = sp::RenderData::Type::Normal;
                render_data.shader = sp::Shader::get("shader/overlay.shader");
                render_data.mesh = sp::MeshData::createQuad(sp::Vector2f(2, 2));
                render_data.color = sp::Color(0,0,0);
#ifdef DEBUG
                render_data.color.a = 0.7;
#endif        
                queue.add(sp::Matrix4x4d::identity(), render_data);
            }

            glDisable(GL_STENCIL_TEST);
            
            mode = Mode::PostShadow;
            recursiveNodeRender(queue, *scene->getRoot());
        }
    }
    
    void recursiveNodeRender(sp::RenderQueue& queue, sp::Node* node)
    {
        if (node->render_data.type == sp::RenderData::Type::None)
        {
            if (mode == Mode::Shadow && dynamic_cast<ShadowCastNode*>(node))
            {
                sp::RenderData render_data;
                render_data.type = sp::RenderData::Type::Normal;
                render_data.shader = node->render_data.shader;
                render_data.mesh = node->render_data.mesh;
                render_data.color = sp::Color(1,1,1);
                if (node->render_data.order == 0)
                    queue.add(node->getGlobalTransform(), render_data);
                else if (node->render_data.order - 1 == light_index)
                    queue.add(node->getGlobalTransform(), render_data);
            }
        }
        else if (node->render_data.mesh)
        {
            if (node->render_data.order < 20)
            {
                if (mode == Mode::PreShadow)
                    queue.add(node->getGlobalTransform(), node->render_data);
            }
            else
            {
                if (mode == Mode::PostShadow)
                    queue.add(node->getGlobalTransform(), node->render_data);
            }
        }

        for(sp::Node* child : node->getChildren())
        {
            recursiveNodeRender(queue, child);
        }
    }

    sp::PList<sp::Node> light_sources;
private:
    sp::P<sp::Scene> single_scene;
    sp::P<sp::Camera> specific_camera;
    
    enum class Mode
    {
        PreShadow,
        Shadow,
        PostShadow,
    } mode;
    int light_index;
};

#endif//SHADOW_RENDER_PASS_H

#pragma once

#include "Runtime/Function/Render/RenderPass.h"

namespace MarsEngine
{
    struct CombineUIPassInitInfo : RenderPassInitInfo
    {
        RHIRenderPass* render_pass;
        RHIImageView*  scene_input_attachment;
        RHIImageView*  ui_input_attachment;
    };

    class CombineUIPass : public RenderPass
    {
    public:
        void initialize(RenderPassInitInfo const* init_info) override final;
        void draw() override final;

        void updateAfterFramebufferRecreate(RHIImageView* scene_input_attachment, RHIImageView* ui_input_attachment);

    private:
        void setupDescriptorSetLayout();
        void setupPipelines();
        void setupDescriptorSet();
    };
} // namespace MarsEngine

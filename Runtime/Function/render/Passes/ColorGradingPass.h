#pragma once

#include "Runtime/Function/Render/RenderPass.h"

namespace MarsEngine
{
    struct ColorGradingPassInitInfo : RenderPassInitInfo
    {
        RHIRenderPass* render_pass;
        RHIImageView* input_attachment;
    };

    class ColorGradingPass : public RenderPass
    {
    public:
        void initialize(RenderPassInitInfo const* init_info) override final;
        void draw() override final;

        void updateAfterFramebufferRecreate(RHIImageView* input_attachment);

    private:
        void setupDescriptorSetLayout();
        void setupPipelines();
        void setupDescriptorSet();
    };
} // namespace MarsEngine

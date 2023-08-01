#pragma once

#include "Runtime/Function/Render/RenderPass.h"

namespace MarsEngine
{
    class WindowUI;

    struct FXAAPassInitInfo : RenderPassInitInfo
    {
        RHIRenderPass* render_pass;
        RHIImageView*  input_attachment;
    };

    class FXAAPass : public RenderPass
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

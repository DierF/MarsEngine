#pragma once

#include "Runtime/Function/Render/RenderPass.h"

namespace MarsEngine
{
    class WindowUI;

    struct UIPassInitInfo : RenderPassInitInfo
    {
        RHIRenderPass* render_pass;
    };

    class UIPass : public RenderPass
    {
    public:
        void initialize(RenderPassInitInfo const* init_info) override final;
        void initializeUIRenderBackend(WindowUI* window_ui) override final;
        void draw() override final;

    private:
        void uploadFonts();

    private:
        WindowUI* m_window_ui;
    };
} // namespace MarsEngine

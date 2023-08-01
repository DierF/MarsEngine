#include "Runtime/Function/Render/RenderPipelineBase.h"
#include "Runtime/Function/Render/DebugDraw/DebugDrawManager.h"
#include "Runtime/Function/Global/GlobalContext.h"
#include "Runtime/Core/Base/Macro.h"

namespace MarsEngine
{
    void RenderPipelineBase::preparePassData(std::shared_ptr<RenderResourceBase> render_resource)
    {
        m_main_camera_pass->preparePassData(render_resource);
        m_pick_pass->preparePassData(render_resource);
        m_directional_light_pass->preparePassData(render_resource);
        m_point_light_shadow_pass->preparePassData(render_resource);
        m_particle_pass->preparePassData(render_resource);
        g_runtime_global_context.m_debugdraw_manager->preparePassData(render_resource);
    }

    void RenderPipelineBase::forwardRender(std::shared_ptr<RHI>                rhi,
                                           std::shared_ptr<RenderResourceBase> render_resource)
    {
    }

    void RenderPipelineBase::deferredRender(std::shared_ptr<RHI>                rhi,
                                            std::shared_ptr<RenderResourceBase> render_resource)
    {
    }

    void RenderPipelineBase::initializeUIRenderBackend(WindowUI* window_ui)
    {
        m_ui_pass->initializeUIRenderBackend(window_ui);
    }
} // namespace MarsEngine

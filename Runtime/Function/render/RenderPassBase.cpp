#include "Runtime/Function/Render/RenderPassBase.h"
#include "Runtime/Core/Base/Macro.h"

namespace MarsEngine
{
    void RenderPassBase::postInitialize() {}
    void RenderPassBase::setCommonInfo(RenderPassCommonInfo common_info)
    {
        m_rhi             = common_info.rhi;
        m_render_resource = common_info.render_resource;
    }
    void RenderPassBase::preparePassData(std::shared_ptr<RenderResourceBase> render_resource) {}
    void RenderPassBase::initializeUIRenderBackend(WindowUI* window_ui) {}
} // namespace MarsEngine

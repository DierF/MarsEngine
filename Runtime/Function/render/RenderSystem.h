#pragma once

#include "Runtime/Function/Render/RenderEntity.h"
#include "Runtime/Function/Render/RenderGuidAllocator.h"
#include "Runtime/Function/Render/RenderSwapContext.h"
#include "Runtime/Function/Render/RenderType.h"

#include <array>
#include <memory>
#include <optional>

namespace MarsEngine
{
    class WindowSystem;
    class RHI;
    class RenderResourceBase;
    class RenderPipelineBase;
    class RenderScene;
    class RenderCamera;
    class WindowUI;
    class DebugDrawManager;

    struct RenderSystemInitInfo
    {
        std::shared_ptr<WindowSystem> window_system;
        std::shared_ptr<DebugDrawManager> debugdraw_manager;
    };

    struct EngineContentViewport
    {
        float x      { 0.f };
        float y      { 0.f };
        float width  { 0.f };
        float height { 0.f };
    };

    class RenderSystem
    {
    public:
        RenderSystem() = default;
        ~RenderSystem();

        void initialize(RenderSystemInitInfo init_info);
        void tick(float delta_time);
        void clear();

        void                          swapLogicRenderData();
        RenderSwapContext&            getSwapContext();
        std::shared_ptr<RenderCamera> getRenderCamera() const;
        std::shared_ptr<RHI>          getRHI() const;

        void      setRenderPipelineType(RENDER_PIPELINE_TYPE pipeline_type);
        void      initializeUIRenderBackend(WindowUI* window_ui);
        void      updateEngineContentViewport(float offset_x, float offset_y, float width, float height);
        uint32_t  getGuidOfPickedMesh(Vec2 const& picked_uv);
        GObjectID getGObjectIDByMeshID(uint32_t mesh_id) const;

        EngineContentViewport getEngineContentViewport() const;

        void createAxis(std::array<RenderEntity, 3> axis_entities, std::array<RenderMeshData, 3> mesh_datas);
        void setVisibleAxis(std::optional<RenderEntity> axis);
        void setSelectedAxis(size_t selected_axis);
        GuidAllocator<GameObjectPartId>& getGOInstanceIdAllocator();
        GuidAllocator<MeshSourceDesc>&   getMeshAssetIdAllocator();

        void clearForLevelReloading();

    private:
        RENDER_PIPELINE_TYPE m_render_pipeline_type {RENDER_PIPELINE_TYPE::DEFERRED_PIPELINE};

        RenderSwapContext m_swap_context;

        std::shared_ptr<RHI>                m_rhi;
        std::shared_ptr<RenderCamera>       m_render_camera;
        std::shared_ptr<RenderScene>        m_render_scene;
        std::shared_ptr<RenderResourceBase> m_render_resource;
        std::shared_ptr<RenderPipelineBase> m_render_pipeline;

        void processSwapData();
    };
} // namespace MarsEngine

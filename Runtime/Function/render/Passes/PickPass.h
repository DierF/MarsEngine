#pragma once

#include "Runtime/Core/Math/Vector2.h"
#include "Runtime/Function/Render/RenderPass.h"

namespace MarsEngine
{
    class RenderResourceBase;

    struct PickPassInitInfo : RenderPassInitInfo
    {
        RHIDescriptorSetLayout* per_mesh_layout;
    };

    class PickPass : public RenderPass
    {
    public:
        void initialize(RenderPassInitInfo const* init_info) override final;
        void postInitialize() override final;
        void preparePassData(std::shared_ptr<RenderResourceBase> render_resource) override final;
        void draw() override final;

        uint32_t pick(Vec2 const& picked_uv);
        void     recreateFramebuffer();

        MeshInefficientPickPerframeStorageBufferObject _mesh_inefficient_pick_perframe_storage_buffer_object;

    private:
        void setupAttachments();
        void setupRenderPass();
        void setupFramebuffer();
        void setupDescriptorSetLayout();
        void setupPipelines();
        void setupDescriptorSet();

    private:
        RHIImage*        _object_id_image = nullptr;
        RHIDeviceMemory* _object_id_image_memory = nullptr;
        RHIImageView*      _object_id_image_view = nullptr;

        RHIDescriptorSetLayout* _per_mesh_layout = nullptr;
    };
} // namespace MarsEngine

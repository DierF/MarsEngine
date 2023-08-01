#pragma once

#include "Runtime/Function/Render/Interface/Rhi.h"
#include "Runtime/Function/Render/DebugDraw/DebugDrawPrimitive.h"
#include "Runtime/Function/Render/DebugDraw/DebugDrawFont.h"
#include "Runtime/Core/Math/MathHeaders.h"

#include <queue>

namespace MarsEngine
{
    class DebugDrawAllocator
    {
    public:
        DebugDrawAllocator() {};
        void initialize(DebugDrawFont* font);
        void destory();
        void tick();
        void clear();
        void clearBuffer();
        
        size_t cacheVertexs(std::vector<DebugDrawVertex> const& vertexs);
        void cacheUniformObject(Mat4 proj_view_matrix);
        size_t cacheUniformDynamicObject(std::vector<std::pair<Mat4, Vec4>> const& model_colors);

        size_t getVertexCacheOffset() const;
        size_t getUniformDynamicCacheOffset() const;
        void allocator();

        RHIBuffer* getVertexBuffer();
        RHIDescriptorSet* &getDescriptorSet();

        RHIBuffer* getSphereVertexBuffer();
        RHIBuffer* getCylinderVertexBuffer();
        RHIBuffer* getCapsuleVertexBuffer();

        size_t const getSphereVertexBufferSize() const;
        size_t const getCylinderVertexBufferSize() const;
        size_t const getCapsuleVertexBufferSize() const;
        size_t const getCapsuleVertexBufferUpSize() const;
        size_t const getCapsuleVertexBufferMidSize() const;
        size_t const getCapsuleVertexBufferDownSize() const;

        size_t const getSizeOfUniformBufferObject() const;
    private:
        std::shared_ptr<RHI> m_rhi;
        struct UniformBufferObject
        {
            Mat4 proj_view_matrix;
        };

        struct alignas(256) UniformBufferDynamicObject
        {
            Mat4 model_matrix;
            Vec4 color;
        };

        struct Resource
        {
            RHIBuffer* buffer = nullptr;
            RHIDeviceMemory* memory = nullptr;
        };

        struct Descriptor
        {
            RHIDescriptorSetLayout* layout = nullptr;
            std::vector<RHIDescriptorSet*> descriptor_set;
        };

        //descriptor
        Descriptor m_descriptor;

        //changeable resource
        Resource m_vertex_resource;
        std::vector<DebugDrawVertex>m_vertex_cache;

        Resource m_uniform_resource;
        UniformBufferObject m_uniform_buffer_object;

        Resource m_uniform_dynamic_resource;
        std::vector<UniformBufferDynamicObject> m_uniform_buffer_dynamic_object_cache;

        //static mesh resource
        Resource m_sphere_resource;
        Resource m_cylinder_resource;
        Resource m_capsule_resource;

        //font resource
        DebugDrawFont* m_font = nullptr;

        //resource deleter
        static uint32_t const k_deferred_delete_resource_frame_count = 5;
        //the count means after count-1 frame will be delete

        uint32_t m_current_frame = 0;
        std::queue<Resource> m_deffer_delete_queue[k_deferred_delete_resource_frame_count];

    private:
        void setupDescriptorSet();
        void prepareDescriptorSet();
        void updateDescriptorSet();
        void flushPendingDelete();
        void unloadMeshBuffer();
        void loadSphereMeshBuffer();
        void loadCylinderMeshBuffer();
        void loadCapsuleMeshBuffer();

        int const m_circle_sample_count = 10;
    };
} // namespace MarsEngine
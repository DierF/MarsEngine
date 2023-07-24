#pragma once

#include "Runtime/Core/Math/AxisAligned.h"
#include "Runtime/Core/Math/Matrix4.h"

#include <cstdint>
#include <vector>

namespace MarsEngine
{
    class RenderEntity
    {
    public:
        uint32_t   m_instance_id {0};
        Math::Mat4 m_model_matrix {Math::Mat4::IDENTITY};

        // mesh
        size_t                  m_mesh_asset_id {0};
        bool                    m_enable_vertex_blending {false};
        std::vector<Math::Mat4> m_joint_matrices;
        Math::AxisAlignedBox    m_bounding_box;

        // material
        size_t     m_material_asset_id {0};
        bool       m_blend {false};
        bool       m_double_sided {false};
        Math::Vec4 m_base_color_factor {1.0f, 1.0f, 1.0f, 1.0f};
        float      m_metallic_factor {1.0f};
        float      m_roughness_factor {1.0f};
        float      m_normal_scale {1.0f};
        float      m_occlusion_strength {1.0f};
        Math::Vec3 m_emissive_factor {0.0f, 0.0f, 0.0f};
    };
} // namespace MarsEngine

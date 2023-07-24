#pragma once

#include "Runtime/Function/Render/Interface/Rhi.h"
#include "Runtime/Core/Math/MathHeaders.h"

#include <array>

namespace MarsEngine
{
    static float const k_debug_draw_infinity_life_time = -2.f;
    static float const k_debug_draw_one_frame = 0.0f;

    enum DebugDrawTimeType : uint8_t
    {
        _debugDrawTimeType_infinity,
        _debugDrawTimeType_one_frame,
        _debugDrawTimeType_common
    };

    enum DebugDrawPrimitiveType : uint8_t
    {
        _debug_draw_primitive_type_point = 0,
        _debug_draw_primitive_type_line,
        _debug_draw_primitive_type_triangle,
        _debug_draw_primitive_type_quad,
        _debug_draw_primitive_type_draw_box,
        _debug_draw_primitive_type_cylinder,
        _debug_draw_primitive_type_sphere,
        _debug_draw_primitive_type_capsule,
        _debug_draw_primitive_type_text,
        k_debug_draw_primitive_type_count
    };
    enum FillMode : uint8_t
    {
        _FillMode_wireframe = 0,
        _FillMode_solid = 1,
        k_FillMode_count,
    };

    struct DebugDrawVertex
    {
        Math::Vec3 pos;
        Math::Vec4 color;
        Math::Vec2 texcoord;
        DebugDrawVertex() { pos = Math::Vec3(-1.0f, -1.0f, -1.0f); color = Math::Vec4(-1.0f, -1.0f, -1.0f, -1.0f); texcoord = Math::Vec2(-1.0f, -1.0f); }

        static std::array<RHIVertexInputBindingDescription, 1> getBindingDescriptions()
        {
            std::array<RHIVertexInputBindingDescription, 1>binding_descriptions;
            binding_descriptions[0].binding = 0;
            binding_descriptions[0].stride = sizeof(DebugDrawVertex);
            binding_descriptions[0].inputRate = RHI_VERTEX_INPUT_RATE_VERTEX;

            return binding_descriptions;
        }

        static std::array<RHIVertexInputAttributeDescription, 3> getAttributeDescriptions()
        {
            std::array<RHIVertexInputAttributeDescription, 3> attribute_descriptions{};

            // position
            attribute_descriptions[0].binding = 0;
            attribute_descriptions[0].location = 0;
            attribute_descriptions[0].format = RHI_FORMAT_R32G32B32_SFLOAT;
            attribute_descriptions[0].offset = offsetof(DebugDrawVertex, pos);

            // color
            attribute_descriptions[1].binding = 0;
            attribute_descriptions[1].location = 1;
            attribute_descriptions[1].format = RHI_FORMAT_R32G32B32A32_SFLOAT;
            attribute_descriptions[1].offset = offsetof(DebugDrawVertex, color);

            // texcoord
            attribute_descriptions[2].binding = 0;
            attribute_descriptions[2].location = 2;
            attribute_descriptions[2].format = RHI_FORMAT_R32G32_SFLOAT;
            attribute_descriptions[2].offset = offsetof(DebugDrawVertex, texcoord);

            return attribute_descriptions;
        }
    };

    class DebugDrawPrimitive
    {
    public:
        DebugDrawTimeType m_time_type{ _debugDrawTimeType_infinity };
        float m_life_time{ 0.f };
        FillMode m_fill_mode{ _FillMode_wireframe };
        bool m_no_depth_test = false;
    
    public:
        bool isTimeOut(float delta_time);
        void setTime(float in_life_time);
    
    private:
        bool m_rendered = false;// for one frame object
    };

    class DebugDrawPoint : public DebugDrawPrimitive
    {
    public:
        DebugDrawVertex m_vertex;
        static DebugDrawPrimitiveType const k_type_enum_value = _debug_draw_primitive_type_point;
    };

    class DebugDrawLine : public DebugDrawPrimitive
    {
    public:
        DebugDrawVertex m_vertex[2];
        static DebugDrawPrimitiveType const k_type_enum_value = _debug_draw_primitive_type_line;
    };

    class DebugDrawTriangle : public DebugDrawPrimitive
    {
    public:
        DebugDrawVertex m_vertex[3];
        static DebugDrawPrimitiveType const k_type_enum_value = _debug_draw_primitive_type_triangle;
    };

    class DebugDrawQuad : public DebugDrawPrimitive
    {
    public:
        DebugDrawVertex m_vertex[4];

        static DebugDrawPrimitiveType const k_type_enum_value = _debug_draw_primitive_type_quad;
    };

    class DebugDrawBox : public DebugDrawPrimitive
    {
    public:
        Math::Vec3 m_center_point;
        Math::Vec3 m_half_extents;
        Math::Vec4 m_color;
        Math::Vec4 m_rotate;

        static DebugDrawPrimitiveType const k_type_enum_value = _debug_draw_primitive_type_draw_box;
    };

    class DebugDrawCylinder : public DebugDrawPrimitive
    {
    public:
        Math::Vec3 m_center;
        Math::Vec4 m_rotate;
        float      m_radius{ 0.f };
        float      m_height{ 0.f };
        Math::Vec4 m_color;

        static DebugDrawPrimitiveType const k_type_enum_value = _debug_draw_primitive_type_cylinder;
    };
    class DebugDrawSphere : public DebugDrawPrimitive
    {
    public:
        Math::Vec3 m_center;
        float      m_radius{ 0.f };
        Math::Vec4 m_color;

        static DebugDrawPrimitiveType const k_type_enum_value = _debug_draw_primitive_type_sphere;
    };

    class DebugDrawCapsule : public DebugDrawPrimitive
    {
    public:
        Math::Vec3 m_center;
        Math::Vec4 m_rotation;
        Math::Vec3 m_scale;
        float      m_radius{ 0.f };
        float      m_height{ 0.f };
        Math::Vec4 m_color;

        static DebugDrawPrimitiveType const k_type_enum_value = _debug_draw_primitive_type_capsule;
    };

    class DebugDrawText : public DebugDrawPrimitive
    {
    public:
        std::string m_content;
        Math::Vec4  m_color;
        Math::Vec3  m_coordinate;
        int         m_size;
        bool        m_is_screen_text;

        static DebugDrawPrimitiveType const k_type_enum_value = _debug_draw_primitive_type_text;
    };
} // namespace MarsEngine
#pragma once

#include "Runtime/Function/Particle/EmitterIdAllocator.h"
#include "Runtime/Resource/ResType/Components/Emitter.h"
#include "Runtime/Core/Math/Matrix4.h"
#include "Runtime/Core/Math/Vector2.h"
#include "Runtime/Core/Math/Vector3.h"
#include "Runtime/Core/Math/Vector4.h"

namespace MarsEngine
{
    struct ParticleEmitterTransformDesc
    {
        ParticleEmitterID m_id;
        Vec4        m_position;
        Mat4        m_rotation;
    };

    struct ParticleEmitterDesc
    {
        Vec4   m_position;
        Mat4   m_rotation;
        Vec4   m_velocity;
        Vec4   m_acceleration;
        Vec3   m_size;
        int          m_emitter_type;
        Vec2   m_life;
        Vec2   m_padding;
        Vec4   m_color;

        ParticleEmitterDesc() = default;

        ParticleEmitterDesc(ParticleComponentRes const& component_res, ParticleEmitterTransformDesc& transform_desc) :
            m_position(transform_desc.m_position), m_rotation(transform_desc.m_rotation),
            m_velocity(component_res.m_velocity), m_acceleration(component_res.m_acceleration),
            m_size(component_res.m_size), m_emitter_type(component_res.m_emitter_type), m_life(component_res.m_life),
            m_color(component_res.m_color)
        {}
    };
} // namespace MarsEngine

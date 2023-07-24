#pragma once

#include "Runtime/Core/Math/Matrix4.h"
#include "Runtime/Core/Math/Vector2.h"
#include "Runtime/Core/Math/Vector4.h"
#include "Runtime/Core/Meta/Reflection/Reflection.h"

namespace MarsEngine
{
    REFLECTION_TYPE(ParticleComponentRes)
        CLASS(ParticleComponentRes, Fields)
    {
        REFLECTION_BODY(ParticleComponentRes);

    public:
        Math::Vec3       m_local_translation; // local translation
        Math::Quaternion m_local_rotation;    // local rotation
        Math::Vec4       m_velocity;          // velocity base & variance
        Math::Vec4       m_acceleration;      // acceleration base & variance
        Math::Vec3       m_size;              // size base & variance
        int              m_emitter_type;
        Math::Vec2       m_life;  // life base & variance
        Math::Vec4       m_color; // color rgba
    };
} // namespace MarsEngine
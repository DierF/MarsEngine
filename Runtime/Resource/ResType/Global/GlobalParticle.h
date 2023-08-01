#pragma once

#include "Runtime/Core/Math/Vector3.h"
#include "Runtime/Core/Meta/Reflection/Reflection.h"

namespace MarsEngine
{
    REFLECTION_TYPE(GlobalParticleRes)
    CLASS(GlobalParticleRes, Fields)
    {
        REFLECTION_BODY(GlobalParticleRes)

    public:
        GlobalParticleRes() {}

        META(Enable)
        int         m_emit_gap;
        int         m_emit_count;
        float       m_time_step;
        float       m_max_life;
        Vec3  m_gravity;
        std::string m_particle_billboard_texture_path;
        std::string m_mars_logo_texture_path;
    };
} // namespace MarsEngine
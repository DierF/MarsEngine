#pragma once

#include "Runtime/Core/Meta/Reflection/Reflection.h"
#include "Runtime/Core/Math/Vector2.h"
#include "Runtime/Core/Math/Vector3.h"

namespace MarsEngine
{
    REFLECTION_TYPE(CameraPose)
    CLASS(CameraPose, Fields)
    {
        REFLECTION_BODY(CameraPose);

    public:
        Vec3 m_position;
        Vec3 m_target;
        Vec3 m_up;
    };

    REFLECTION_TYPE(CameraConfig)
    CLASS(CameraConfig, Fields)
    {
        REFLECTION_BODY(CameraConfig);

    public:
        CameraPose m_pose;
        Vec2 m_aspect;
        float      m_z_far;
        float      m_z_near;
    };
} // namespace MarsEngine
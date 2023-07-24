#pragma once

#include "Runtime/Core/Math/Quaternion.h"
#include "Runtime/Core/Meta/Reflection/Reflection.h"

namespace MarsEngine
{
    REFLECTION_TYPE(CameraParameter)
    CLASS(CameraParameter, Fields)
    {
        REFLECTION_BODY(CameraParameter);

    public:
        float m_fov {50.f};

        virtual ~CameraParameter() {}
    };

    REFLECTION_TYPE(FirstPersonCameraParameter)
    CLASS(FirstPersonCameraParameter : public CameraParameter, Fields)
    {
        REFLECTION_BODY(FirstPersonCameraParameter);

    public:
        float m_vertical_offset {0.6f};
    };

    REFLECTION_TYPE(ThirdPersonCameraParameter)
    CLASS(ThirdPersonCameraParameter : public CameraParameter, WhiteListFields)
    {
        REFLECTION_BODY(ThirdPersonCameraParameter);

    public:
        META(Enable)
        float m_horizontal_offset {3.f};
        META(Enable)
        float      m_vertical_offset {2.5f};
        Math::Quaternion m_cursor_pitch;
        Math::Quaternion m_cursor_yaw;
    };

    REFLECTION_TYPE(FreeCameraParameter)
    CLASS(FreeCameraParameter : public CameraParameter, Fields)
    {
        REFLECTION_BODY(FreeCameraParameter);

    public:
        float m_speed {1.f};
    };

    REFLECTION_TYPE(CameraComponentRes)
    CLASS(CameraComponentRes, Fields)
    {
        REFLECTION_BODY(CameraComponentRes);

    public:
        Reflection::ReflectionPtr<CameraParameter> m_parameter;

        CameraComponentRes() = default;
        CameraComponentRes(const CameraComponentRes& res);

        ~CameraComponentRes();
    };
} // namespace MarsEngine
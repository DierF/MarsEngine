#pragma once

#include "Runtime/Core/Math/Vector3.h"
#include "Runtime/Resource/ResType/Components/Camera.h"
#include "Runtime/Function/Framework/Component/Component.h"

namespace MarsEngine
{
    class RenderCamera;

    enum class CameraMode : unsigned char
    {
        third_person,
        first_person,
        free,
        invalid
    };

    REFLECTION_TYPE(CameraComponent)
    CLASS(CameraComponent : public Component, WhiteListFields)
    {
        REFLECTION_BODY(CameraComponent)

    public:
        CameraComponent() = default;

        void postLoadResource(std::weak_ptr<GObject> parent_object) override;

        void tick(float delta_time) override;

        CameraMode getCameraMode() const { return m_camera_mode; }
        void setCameraMode(CameraMode mode) { m_camera_mode = mode; }
        Math::Vec3 getPosition() const { return m_position; }
        Math::Vec3 getForward() const { return m_forward; }

    private:
        void tickFirstPersonCamera(float delta_time);
        void tickThirdPersonCamera(float delta_time);
        void tickFreeCamera(float delta_time);

        META(Enable)
        CameraComponentRes m_camera_res;

        CameraMode m_camera_mode {CameraMode::invalid};

        Math::Vec3 m_position;

        Math::Vec3 m_forward {Math::Vec3::NEGATIVE_UNIT_Y};
        Math::Vec3 m_up {Math::Vec3::UNIT_Z};
        Math::Vec3 m_left {Math::Vec3::UNIT_X};
    };
} // namespace MarsEngine

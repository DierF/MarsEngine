#pragma once

#include "Runtime/Core/Math/MathHeaders.h"

#include <mutex>

namespace MarsEngine
{
    enum class RenderCameraType : int
    {
        Editor,
        Motor
    };

    class RenderCamera
    {
    public:
        RenderCameraType m_current_camera_type {RenderCameraType::Editor};

        static Math::Vec3 const X, Y, Z;

        Math::Vec3       m_position {0.0f, 0.0f, 0.0f};
        Math::Quaternion m_rotation {Math::Quaternion::IDENTITY};
        Math::Quaternion m_invRotation {Math::Quaternion::IDENTITY};
        float            m_znear {1000.0f};
        float            m_zfar {0.1f};
        Math::Vec3       m_up_axis {Z};

        static constexpr float MIN_FOV {10.0f};
        static constexpr float MAX_FOV {89.0f};
        static constexpr int   MAIN_VIEW_MATRIX_INDEX {0};

        std::vector<Math::Mat4> m_view_matrices {Math::Mat4::IDENTITY};

        void setCurrentCameraType(RenderCameraType type);
        void setMainViewMatrix(Math::Mat4 const& view_matrix, RenderCameraType type = RenderCameraType::Editor);

        void move(Math::Vec3 delta);
        void rotate(Math::Vec2 delta);
        void zoom(float offset);
        void lookAt(Math::Vec3 const& position, Math::Vec3 const& target, Math::Vec3 const& up);

        void setAspect(float aspect);
        void setFOVx(float fovx) { m_fovx = fovx; }

        Math::Vec3    position() const { return m_position; }
        Math::Quaternion rotation() const { return m_rotation; }

        Math::Vec3   forward() const { return (m_invRotation * Y); }
        Math::Vec3   up() const { return (m_invRotation * Z); }
        Math::Vec3   right() const { return (m_invRotation * X); }
        Math::Vec2   getFOV() const { return {m_fovx, m_fovy}; }
        Math::Mat4 getViewMatrix();
        Math::Mat4 getPersProjMatrix() const;
        Math::Mat4 getLookAtMatrix() const { return Math::makeLookAtMatrix(position(), position() + forward(), up()); }
        float     getFovYDeprecated() const { return m_fovy; }

    protected:
        float m_aspect {0.f};
        float m_fovx   { (float)Math::Degree(89.f) };
        float m_fovy   {0.f};

        std::mutex m_view_matrix_mutex;
    };

    inline Math::Vec3 const RenderCamera::X = {1.0f, 0.0f, 0.0f};
    inline Math::Vec3 const RenderCamera::Y = {0.0f, 1.0f, 0.0f};
    inline Math::Vec3 const RenderCamera::Z = {0.0f, 0.0f, 1.0f};

} // namespace MarsEngine

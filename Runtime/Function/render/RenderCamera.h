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

        static Vec3 const X, Y, Z;

        Vec3       m_position {0.0f, 0.0f, 0.0f};
        Quaternion m_rotation {Quaternion::IDENTITY};
        Quaternion m_invRotation {Quaternion::IDENTITY};
        float            m_znear {1000.0f};
        float            m_zfar {0.1f};
        Vec3       m_up_axis {Z};

        static constexpr float MIN_FOV {10.0f};
        static constexpr float MAX_FOV {89.0f};
        static constexpr int   MAIN_VIEW_MATRIX_INDEX {0};

        std::vector<Mat4> m_view_matrices {Mat4::IDENTITY};

        void setCurrentCameraType(RenderCameraType type);
        void setMainViewMatrix(Mat4 const& view_matrix, RenderCameraType type = RenderCameraType::Editor);

        void move(Vec3 delta);
        void rotate(Vec2 delta);
        void zoom(float offset);
        void lookAt(Vec3 const& position, Vec3 const& target, Vec3 const& up);

        void setAspect(float aspect);
        void setFOVx(float fovx) { m_fovx = fovx; }

        Vec3    position() const { return m_position; }
        Quaternion rotation() const { return m_rotation; }

        Vec3   forward() const { return (m_invRotation * Y); }
        Vec3   up() const { return (m_invRotation * Z); }
        Vec3   right() const { return (m_invRotation * X); }
        Vec2   getFOV() const { return {m_fovx, m_fovy}; }
        Mat4 getViewMatrix();
        Mat4 getPersProjMatrix() const;
        Mat4 getLookAtMatrix() const { return Math::makeLookAtMatrix(position(), position() + forward(), up()); }
        float     getFovYDeprecated() const { return m_fovy; }

    protected:
        float m_aspect {0.f};
        float m_fovx   { (float)Degree(89.f) };
        float m_fovy   {0.f};

        std::mutex m_view_matrix_mutex;
    };

    inline Vec3 const RenderCamera::X = {1.0f, 0.0f, 0.0f};
    inline Vec3 const RenderCamera::Y = {0.0f, 1.0f, 0.0f};
    inline Vec3 const RenderCamera::Z = {0.0f, 0.0f, 1.0f};

} // namespace MarsEngine

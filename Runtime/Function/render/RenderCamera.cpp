#include "Runtime/Function/Render/RenderCamera.h"

namespace MarsEngine
{
    void RenderCamera::setCurrentCameraType(RenderCameraType type)
    {
        std::lock_guard<std::mutex> lock_guard(m_view_matrix_mutex);
        m_current_camera_type = type;
    }

    void RenderCamera::setMainViewMatrix(Mat4 const& view_matrix, RenderCameraType type)
    {
        std::lock_guard<std::mutex> lock_guard(m_view_matrix_mutex);
        m_current_camera_type                   = type;
        m_view_matrices[MAIN_VIEW_MATRIX_INDEX] = view_matrix;

        Vec3 s  = Vec3(view_matrix[0][0], view_matrix[0][1], view_matrix[0][2]);
        Vec3 u  = Vec3(view_matrix[1][0], view_matrix[1][1], view_matrix[1][2]);
        Vec3 f  = Vec3(-view_matrix[2][0], -view_matrix[2][1], -view_matrix[2][2]);
        m_position = s * (-view_matrix[0][3]) + u * (-view_matrix[1][3]) + f * view_matrix[2][3];
    }

    void RenderCamera::move(Vec3 delta) { m_position += delta; }

    void RenderCamera::rotate(Vec2 delta)
    {
        // rotation around x, y axis
        delta = Vec2((float)Radian(Degree(delta.x)), (float)Radian(Degree(delta.y)));

        // limit pitch
        float dot = m_up_axis.dotProduct(forward());
        if ((dot < -0.99f && delta.x > 0.0f) || // angle nearing 180 degrees
            (dot > 0.99f && delta.x < 0.0f))    // angle nearing 0 degrees
            delta.x = 0.0f;

        // pitch is relative to current sideways rotation
        // yaw happens independently
        // this prevents roll
        Quaternion pitch, yaw;
        pitch.fromAngleAxis(Radian(delta.x), X);
        yaw.fromAngleAxis(Radian(delta.y), Z);

        m_rotation = pitch * m_rotation * yaw;

        m_invRotation = m_rotation.conjugate();
    }

    void RenderCamera::zoom(float offset)
    {
        // > 0 = zoom in (decrease FOV by <offset> angles)
        m_fovx = Math::clamp(m_fovx - offset, MIN_FOV, MAX_FOV);
    }

    void RenderCamera::lookAt(Vec3 const& position, Vec3 const& target, Vec3 const& up)
    {
        m_position = position;

        // model rotation
        // maps vectors to camera space (x, y, z)
        Vec3 forward = (target - position).normalisedCopy();
        m_rotation      = forward.getRotationTo(Y);

        // correct the up vector
        // the cross product of non-orthogonal vectors is not normalized
        Vec3 right  = forward.crossProduct(up.normalisedCopy()).normalisedCopy();
        Vec3 orthUp = right.crossProduct(forward);

        Quaternion upRotation = (m_rotation * orthUp).getRotationTo(Z);

        m_rotation = Quaternion(upRotation) * m_rotation;

        // inverse of the model rotation
        // maps camera space vectors to model vectors
        m_invRotation = m_rotation.conjugate();
    }

    Mat4 RenderCamera::getViewMatrix()
    {
        std::lock_guard<std::mutex> lock_guard(m_view_matrix_mutex);
        auto                        view_matrix = Mat4::IDENTITY;
        switch (m_current_camera_type)
        {
            case RenderCameraType::Editor:
                view_matrix = Math::makeLookAtMatrix(position(), position() + forward(), up());
                break;
            case RenderCameraType::Motor:
                view_matrix = m_view_matrices[MAIN_VIEW_MATRIX_INDEX];
                break;
            default:
                break;
        }
        return view_matrix;
    }

    Mat4 RenderCamera::getPersProjMatrix() const
    {
        Mat4 fix_mat(1, 0, 0, 0, 0, -1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
        Mat4 proj_mat = fix_mat * Math::makePerspectiveMatrix(Radian(Degree(m_fovy)),
                                                                    m_aspect, m_znear, m_zfar);

        return proj_mat;
    }

    void RenderCamera::setAspect(float aspect)
    {
        m_aspect = aspect;

        // 1 / tan(fovy * 0.5) / aspect = 1 / tan(fovx * 0.5)
        // 1 / tan(fovy * 0.5) = aspect / tan(fovx * 0.5)
        // tan(fovy * 0.5) = tan(fovx * 0.5) / aspect

        m_fovy = (float)Degree(Math::atan(Math::tan(Radian(Degree(m_fovx) * 0.5f)) / m_aspect) * 2.0f);
    }
} // namespace MarsEngine

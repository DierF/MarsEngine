#include "pch.h"

#include "Matrix4.h"

namespace MarsEngine
{
namespace Math
{
    Mat4 const Mat4::ZERO(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

    Mat4 const Mat4::ZEROAFFINE(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1);

    Mat4 const Mat4::IDENTITY(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);

    //-----------------------------------------------------------------------
    Mat4 Mat4::adjoint() const
    {
        return Mat4(getMinor(1, 2, 3, 1, 2, 3),
            -getMinor(0, 2, 3, 1, 2, 3),
            getMinor(0, 1, 3, 1, 2, 3),
            -getMinor(0, 1, 2, 1, 2, 3),

            -getMinor(1, 2, 3, 0, 2, 3),
            getMinor(0, 2, 3, 0, 2, 3),
            -getMinor(0, 1, 3, 0, 2, 3),
            getMinor(0, 1, 2, 0, 2, 3),

            getMinor(1, 2, 3, 0, 1, 3),
            -getMinor(0, 2, 3, 0, 1, 3),
            getMinor(0, 1, 3, 0, 1, 3),
            -getMinor(0, 1, 2, 0, 1, 3),

            -getMinor(1, 2, 3, 0, 1, 2),
            getMinor(0, 2, 3, 0, 1, 2),
            -getMinor(0, 1, 3, 0, 1, 2),
            getMinor(0, 1, 2, 0, 1, 2));
    }

    //-----------------------------------------------------------------------
    Mat4 Mat4::inverseAffine(void) const
    {
        assert(isAffine());

        float m10 = m_mat[1][0], m11 = m_mat[1][1], m12 = m_mat[1][2];
        float m20 = m_mat[2][0], m21 = m_mat[2][1], m22 = m_mat[2][2];

        float t00 = m22 * m11 - m21 * m12;
        float t10 = m20 * m12 - m22 * m10;
        float t20 = m21 * m10 - m20 * m11;

        float m00 = m_mat[0][0], m01 = m_mat[0][1], m02 = m_mat[0][2];

        float inv_det = 1 / (m00 * t00 + m01 * t10 + m02 * t20);

        t00 *= inv_det;
        t10 *= inv_det;
        t20 *= inv_det;

        m00 *= inv_det;
        m01 *= inv_det;
        m02 *= inv_det;

        float r00 = t00;
        float r01 = m02 * m21 - m01 * m22;
        float r02 = m01 * m12 - m02 * m11;

        float r10 = t10;
        float r11 = m00 * m22 - m02 * m20;
        float r12 = m02 * m10 - m00 * m12;

        float r20 = t20;
        float r21 = m01 * m20 - m00 * m21;
        float r22 = m00 * m11 - m01 * m10;

        float m03 = m_mat[0][3], m13 = m_mat[1][3], m23 = m_mat[2][3];

        float r03 = -(r00 * m03 + r01 * m13 + r02 * m23);
        float r13 = -(r10 * m03 + r11 * m13 + r12 * m23);
        float r23 = -(r20 * m03 + r21 * m13 + r22 * m23);

        return Mat4(r00, r01, r02, r03, r10, r11, r12, r13, r20, r21, r22, r23, 0, 0, 0, 1);
    }
    //-----------------------------------------------------------------------
    void Mat4::makeTransform(Vec3 const& position, Vec3 const& scale, Quaternion const& orientation)
    {
        // Ordering:
        //    1. Scale
        //    2. Rotate
        //    3. Translate

        Mat3 rot3x3;
        orientation.toRotationMatrix(rot3x3);

        // Set up final matrix with scale, rotation and translation
        m_mat[0][0] = scale.x * rot3x3[0][0];
        m_mat[0][1] = scale.y * rot3x3[0][1];
        m_mat[0][2] = scale.z * rot3x3[0][2];
        m_mat[0][3] = position.x;
        m_mat[1][0] = scale.x * rot3x3[1][0];
        m_mat[1][1] = scale.y * rot3x3[1][1];
        m_mat[1][2] = scale.z * rot3x3[1][2];
        m_mat[1][3] = position.y;
        m_mat[2][0] = scale.x * rot3x3[2][0];
        m_mat[2][1] = scale.y * rot3x3[2][1];
        m_mat[2][2] = scale.z * rot3x3[2][2];
        m_mat[2][3] = position.z;

        // No projection term
        m_mat[3][0] = 0;
        m_mat[3][1] = 0;
        m_mat[3][2] = 0;
        m_mat[3][3] = 1;
    }

    //-----------------------------------------------------------------------
    void Mat4::makeInverseTransform(Vec3 const& position, Vec3 const& scale, Quaternion const& orientation)
    {
        // Invert the parameters
        Vec3    inv_translate = -position;
        Vec3    inv_scale(1 / scale.x, 1 / scale.y, 1 / scale.z);
        Quaternion inv_rot = orientation.inverse();

        // Because we're inverting, order is translation, rotation, scale
        // So make translation relative to scale & rotation
        inv_translate = inv_rot * inv_translate; // rotate
        inv_translate *= inv_scale;              // scale

        // Next, make a 3x3 rotation matrix
        Mat3 rot3x3;
        inv_rot.toRotationMatrix(rot3x3);

        // Set up final matrix with scale, rotation and translation
        m_mat[0][0] = inv_scale.x * rot3x3[0][0];
        m_mat[0][1] = inv_scale.x * rot3x3[0][1];
        m_mat[0][2] = inv_scale.x * rot3x3[0][2];
        m_mat[0][3] = inv_translate.x;
        m_mat[1][0] = inv_scale.y * rot3x3[1][0];
        m_mat[1][1] = inv_scale.y * rot3x3[1][1];
        m_mat[1][2] = inv_scale.y * rot3x3[1][2];
        m_mat[1][3] = inv_translate.y;
        m_mat[2][0] = inv_scale.z * rot3x3[2][0];
        m_mat[2][1] = inv_scale.z * rot3x3[2][1];
        m_mat[2][2] = inv_scale.z * rot3x3[2][2];
        m_mat[2][3] = inv_translate.z;

        // No projection term
        m_mat[3][0] = 0;
        m_mat[3][1] = 0;
        m_mat[3][2] = 0;
        m_mat[3][3] = 1;
    }
    //-----------------------------------------------------------------------
    void Mat4::decomposition(Vec3& position, Vec3& scale, Quaternion& orientation) const
    {
        // ASSERT(isAffine());

        Mat3 m3x3;
        extract3x3Matrix(m3x3);

        Mat3 mat_q;
        Vec3   vec_u;
        m3x3.calculateQDUDecomposition(mat_q, scale, vec_u);

        orientation = Quaternion(mat_q);
        position = Vec3(m_mat[0][3], m_mat[1][3], m_mat[2][3]);
    }

    //-----------------------------------------------------------------------
    void Mat4::decompositionWithoutScale(class Vec3& position, class Quaternion& rotation) const
    {
        // ASSERT(isAffine());

        Mat3 m3x3;
        extract3x3Matrix(m3x3);

        Mat3 mat_q;
        Vec3   vec_u;
        Vec3   scale;
        m3x3.calculateQDUDecomposition(mat_q, scale, vec_u);

        rotation = Quaternion(mat_q);
        position = Vec3(m_mat[0][3], m_mat[1][3], m_mat[2][3]);
    }

    Vec4 operator*(Vec4 const& v, Mat4 const& mat)
    {
        return Vec4(v.x * mat[0][0] + v.y * mat[1][0] + v.z * mat[2][0] + v.w * mat[3][0],
            v.x * mat[0][1] + v.y * mat[1][1] + v.z * mat[2][1] + v.w * mat[3][1],
            v.x * mat[0][2] + v.y * mat[1][2] + v.z * mat[2][2] + v.w * mat[3][2],
            v.x * mat[0][3] + v.y * mat[1][3] + v.z * mat[2][3] + v.w * mat[3][3]);
    }
} // namespace Math
} // namespace MarsEngine
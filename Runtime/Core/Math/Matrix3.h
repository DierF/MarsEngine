#pragma once

#include "Runtime/Core/Math/Math.h"
#include "Runtime/Core/Math/Degree.h"
#include "Runtime/Core/Math/Radian.h"
#include "Runtime/Core/Math/Quaternion.h"
#include "Runtime/Core/Math/Vector3.h"

#include <cstring>

// NB All code adapted from Wild Magic 0.2 Matrix math (free source code)
// http://www.geometrictools.com/

// NOTE.  The (x,y,z) coordinate system is assumed to be right-handed.
// Coordinate axis rotation matrices are of the form
//   RX =    1       0       0
//           0     cos(t) -sin(t)
//           0     sin(t)  cos(t)
// where t > 0 indicates a counterclockwise rotation in the yz-plane
//   RY =  cos(t)    0     sin(t)
//           0       1       0
//        -sin(t)    0     cos(t)
// where t > 0 indicates a counterclockwise rotation in the zx-plane
//   RZ =  cos(t) -sin(t)    0
//         sin(t)  cos(t)    0
//           0       0       1
// where t > 0 indicates a counterclockwise rotation in the xy-plane.

namespace MarsEngine
{
namespace Math
{
    /** A 3x3 matrix which can represent rotations around axes.
    @par
    The coordinate system is assumed to be <b>right-handed</b>.
    */
    class Mat3
    {
    public:
        float m_mat[3][3];

    public:
        /** Default constructor.
        @note
        It does <b>NOT</b> initialize the matrix for efficiency.
        */
        Mat3() { operator=(IDENTITY); }

        explicit Mat3(float arr[3][3])
        {
            memcpy(m_mat[0], arr[0], 3 * sizeof(float));
            memcpy(m_mat[1], arr[1], 3 * sizeof(float));
            memcpy(m_mat[2], arr[2], 3 * sizeof(float));
        }

        Mat3(float(&float_array)[9])
        {
            m_mat[0][0] = float_array[0];
            m_mat[0][1] = float_array[1];
            m_mat[0][2] = float_array[2];
            m_mat[1][0] = float_array[3];
            m_mat[1][1] = float_array[4];
            m_mat[1][2] = float_array[5];
            m_mat[2][0] = float_array[6];
            m_mat[2][1] = float_array[7];
            m_mat[2][2] = float_array[8];
        }

        Mat3(float entry00,
            float entry01,
            float entry02,
            float entry10,
            float entry11,
            float entry12,
            float entry20,
            float entry21,
            float entry22)
        {
            m_mat[0][0] = entry00;
            m_mat[0][1] = entry01;
            m_mat[0][2] = entry02;
            m_mat[1][0] = entry10;
            m_mat[1][1] = entry11;
            m_mat[1][2] = entry12;
            m_mat[2][0] = entry20;
            m_mat[2][1] = entry21;
            m_mat[2][2] = entry22;
        }

        Mat3(Vec3 const& row0, Vec3 const& row1, Vec3 const& row2)
        {
            m_mat[0][0] = row0.x;
            m_mat[0][1] = row0.y;
            m_mat[0][2] = row0.z;
            m_mat[1][0] = row1.x;
            m_mat[1][1] = row1.y;
            m_mat[1][2] = row1.z;
            m_mat[2][0] = row2.x;
            m_mat[2][1] = row2.y;
            m_mat[2][2] = row2.z;
        }

        Mat3(const Quaternion& q)
        {
            float yy = q.y * q.y;
            float zz = q.z * q.z;
            float xy = q.x * q.y;
            float zw = q.z * q.w;
            float xz = q.x * q.z;
            float yw = q.y * q.w;
            float xx = q.x * q.x;
            float yz = q.y * q.z;
            float xw = q.x * q.w;

            m_mat[0][0] = 1 - 2 * yy - 2 * zz;
            m_mat[0][1] = 2 * xy + 2 * zw;
            m_mat[0][2] = 2 * xz - 2 * yw;

            m_mat[1][0] = 2 * xy - 2 * zw;
            m_mat[1][1] = 1 - 2 * xx - 2 * zz;
            m_mat[1][2] = 2 * yz + 2 * xw;

            m_mat[2][0] = 2 * xz + 2 * yw;
            m_mat[2][1] = 2 * yz - 2 * xw;
            m_mat[2][2] = 1 - 2 * xx - 2 * yy;
        }

        void fromData(float(&float_array)[9])
        {
            m_mat[0][0] = float_array[0];
            m_mat[0][1] = float_array[1];
            m_mat[0][2] = float_array[2];
            m_mat[1][0] = float_array[3];
            m_mat[1][1] = float_array[4];
            m_mat[1][2] = float_array[5];
            m_mat[2][0] = float_array[6];
            m_mat[2][1] = float_array[7];
            m_mat[2][2] = float_array[8];
        }

        void toData(float(&float_array)[9]) const
        {
            float_array[0] = m_mat[0][0];
            float_array[1] = m_mat[0][1];
            float_array[2] = m_mat[0][2];
            float_array[3] = m_mat[1][0];
            float_array[4] = m_mat[1][1];
            float_array[5] = m_mat[1][2];
            float_array[6] = m_mat[2][0];
            float_array[7] = m_mat[2][1];
            float_array[8] = m_mat[2][2];
        }

        // member access, allows use of construct mat[r][c]
        float* operator[](size_t row_index) const { return (float*)m_mat[row_index]; }

        Vec3 getColumn(size_t col_index) const
        {
            assert(0 <= col_index && col_index < 3);
            return Vec3(m_mat[0][col_index], m_mat[1][col_index], m_mat[2][col_index]);
        }

        void setColumn(size_t iCol, Vec3 const& vec);
        void fromAxes(Vec3 const& x_axis, Vec3 const& y_axis, Vec3 const& z_axis);

        // assignment and comparison
        bool operator==(const Mat3& rhs) const
        {
            for (size_t row_index = 0; row_index < 3; row_index++)
            {
                for (size_t col_index = 0; col_index < 3; col_index++)
                {
                    if (m_mat[row_index][col_index] != rhs.m_mat[row_index][col_index])
                        return false;
                }
            }

            return true;
        }

        bool operator!=(const Mat3& rhs) const { return !operator==(rhs); }

        // arithmetic operations
        Mat3 operator+(const Mat3& rhs) const
        {
            Mat3 sum;
            for (size_t row_index = 0; row_index < 3; row_index++)
            {
                for (size_t col_index = 0; col_index < 3; col_index++)
                {
                    sum.m_mat[row_index][col_index] = m_mat[row_index][col_index] + rhs.m_mat[row_index][col_index];
                }
            }
            return sum;
        }

        Mat3 operator-(const Mat3& rhs) const
        {
            Mat3 diff;
            for (size_t row_index = 0; row_index < 3; row_index++)
            {
                for (size_t col_index = 0; col_index < 3; col_index++)
                {
                    diff.m_mat[row_index][col_index] = m_mat[row_index][col_index] - rhs.m_mat[row_index][col_index];
                }
            }
            return diff;
        }

        Mat3 operator*(const Mat3& rhs) const
        {
            Mat3 prod;
            for (size_t row_index = 0; row_index < 3; row_index++)
            {
                for (size_t col_index = 0; col_index < 3; col_index++)
                {
                    prod.m_mat[row_index][col_index] = m_mat[row_index][0] * rhs.m_mat[0][col_index] +
                        m_mat[row_index][1] * rhs.m_mat[1][col_index] +
                        m_mat[row_index][2] * rhs.m_mat[2][col_index];
                }
            }
            return prod;
        }

        // matrix * vector [3x3 * 3x1 = 3x1]
        Vec3 operator*(Vec3 const& rhs) const
        {
            Vec3 prod;
            for (size_t row_index = 0; row_index < 3; row_index++)
            {
                prod[row_index] =
                    m_mat[row_index][0] * rhs.x + m_mat[row_index][1] * rhs.y + m_mat[row_index][2] * rhs.z;
            }
            return prod;
        }

        // vector * matrix [1x3 * 3x3 = 1x3]
        friend Vec3 operator*(Vec3 const& point, const Mat3& rhs)
        {
            Vec3 prod;
            for (size_t row_index = 0; row_index < 3; row_index++)
            {
                prod[row_index] = point.x * rhs.m_mat[0][row_index] + point.y * rhs.m_mat[1][row_index] +
                    point.z * rhs.m_mat[2][row_index];
            }
            return prod;
        }

        Mat3 operator-() const
        {
            Mat3 neg;
            for (size_t row_index = 0; row_index < 3; row_index++)
            {
                for (size_t col_index = 0; col_index < 3; col_index++)
                    neg[row_index][col_index] = -m_mat[row_index][col_index];
            }
            return neg;
        }

        // matrix * scalar
        Mat3 operator*(float scalar) const
        {
            Mat3 prod;
            for (size_t row_index = 0; row_index < 3; row_index++)
            {
                for (size_t col_index = 0; col_index < 3; col_index++)
                    prod[row_index][col_index] = scalar * m_mat[row_index][col_index];
            }
            return prod;
        }

        // scalar * matrix
        friend Mat3 operator*(float scalar, const Mat3& rhs)
        {
            Mat3 prod;
            for (size_t row_index = 0; row_index < 3; row_index++)
            {
                for (size_t col_index = 0; col_index < 3; col_index++)
                    prod[row_index][col_index] = scalar * rhs.m_mat[row_index][col_index];
            }
            return prod;
        }

        // utilities
        Mat3 transpose() const
        {
            Mat3 transpose_v;
            for (size_t row_index = 0; row_index < 3; row_index++)
            {
                for (size_t col_index = 0; col_index < 3; col_index++)
                    transpose_v[row_index][col_index] = m_mat[col_index][row_index];
            }
            return transpose_v;
        }

        bool inverse(Mat3& inv_mat, float fTolerance = 1e-06) const
        {
            // Invert a 3x3 using cofactors.  This is about 8 times faster than
            // the Numerical Recipes code which uses Gaussian elimination.

            float det = determinant();
            if (std::fabs(det) <= fTolerance)
                return false;

            inv_mat[0][0] = m_mat[1][1] * m_mat[2][2] - m_mat[1][2] * m_mat[2][1];
            inv_mat[0][1] = m_mat[0][2] * m_mat[2][1] - m_mat[0][1] * m_mat[2][2];
            inv_mat[0][2] = m_mat[0][1] * m_mat[1][2] - m_mat[0][2] * m_mat[1][1];
            inv_mat[1][0] = m_mat[1][2] * m_mat[2][0] - m_mat[1][0] * m_mat[2][2];
            inv_mat[1][1] = m_mat[0][0] * m_mat[2][2] - m_mat[0][2] * m_mat[2][0];
            inv_mat[1][2] = m_mat[0][2] * m_mat[1][0] - m_mat[0][0] * m_mat[1][2];
            inv_mat[2][0] = m_mat[1][0] * m_mat[2][1] - m_mat[1][1] * m_mat[2][0];
            inv_mat[2][1] = m_mat[0][1] * m_mat[2][0] - m_mat[0][0] * m_mat[2][1];
            inv_mat[2][2] = m_mat[0][0] * m_mat[1][1] - m_mat[0][1] * m_mat[1][0];

            float inv_det = 1.0f / det;
            for (size_t row_index = 0; row_index < 3; row_index++)
            {
                for (size_t col_index = 0; col_index < 3; col_index++)
                    inv_mat[row_index][col_index] *= inv_det;
            }

            return true;
        }

        Mat3 inverse(float tolerance = 1e-06) const
        {
            Mat3 inv = ZERO;
            inverse(inv, tolerance);
            return inv;
        }

        float determinant() const
        {
            float cofactor00 = m_mat[1][1] * m_mat[2][2] - m_mat[1][2] * m_mat[2][1];
            float cofactor10 = m_mat[1][2] * m_mat[2][0] - m_mat[1][0] * m_mat[2][2];
            float cofactor20 = m_mat[1][0] * m_mat[2][1] - m_mat[1][1] * m_mat[2][0];

            float det = m_mat[0][0] * cofactor00 + m_mat[0][1] * cofactor10 + m_mat[0][2] * cofactor20;

            return det;
        }

        void calculateQDUDecomposition(Mat3& out_Q, Vec3& out_D, Vec3& out_U) const;

        // matrix must be orthonormal
        void toAngleAxis(Vec3& axis, Radian& angle) const;
        void toAngleAxis(Vec3& axis, Degree& angle) const
        {
            Radian r;
            toAngleAxis(axis, r);
            angle = r;
        }
        void fromAngleAxis(Vec3 const& axis, Radian const& radian);

        static Mat3 scale(Vec3 const& scale)
        {
            Mat3 mat = ZERO;

            mat.m_mat[0][0] = scale.x;
            mat.m_mat[1][1] = scale.y;
            mat.m_mat[2][2] = scale.z;

            return mat;
        }

        static const Mat3 ZERO;
        static const Mat3 IDENTITY;
    };
} // namespace Math
} // namespace MarsEngine
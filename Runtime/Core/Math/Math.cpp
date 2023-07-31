#include "Runtime/Core/Math/Math.h"
#include "Runtime/Core/Math/Radian.h"
#include "Runtime/Core/Math/Degree.h"
#include "Runtime/Core/Math/Matrix4.h"

namespace MarsEngine
{
namespace Math
{
    Angle::operator Radian() const
    {
        return Radian(angleUnitsToRadians(m_angle));
    }

    Angle::operator Degree() const
    {
        return Degree(angleUnitsToDegrees(m_angle));
    }

    float abs(float value)
    {
        return std::fabs(value);
    }

    bool isNan(float f)
    {
        return std::isnan(f);
    }

    float sqr(float value)
    {
        return value * value;
    }

    float sqrt(float fValue)
    {
        return std::sqrt(fValue);
    }

    float invSqrt(float value)
    {
        return 1.f / sqrt(value);
    }

    bool realEqual(float a, float b, float tolerance /* = std::numeric_limits<float>::epsilon() */)
    {
        return std::fabs(b - a) <= tolerance;
    }

    float clamp(float v, float min, float max)
    {
        return std::clamp(v, min, max);
    }

    float getMaxElement(float x, float y, float z)
    {
        return std::max({ x, y, z });
    }

    float degreesToRadians(float degrees)
    {
        return degrees * fDeg2Rad;
    }

    float radiansToDegrees(float radians)
    {
        return radians * fRad2Deg;
    }

    float angleUnitsToRadians(float angleunits)
    {
        if (s_AngleUnit == AngleUnit::AU_DEGREE)
            return angleunits * fDeg2Rad;

        return angleunits;
    }

    float radiansToAngleUnits(float radians)
    {
        if (s_AngleUnit == AngleUnit::AU_DEGREE)
            return radians * fRad2Deg;

        return radians;
    }

    float angleUnitsToDegrees(float angleunits)
    {
        if (s_AngleUnit == AngleUnit::AU_RADIAN)
            return angleunits * fRad2Deg;

        return angleunits;
    }

    float degreesToAngleUnits(float degrees)
    {
        if (s_AngleUnit == AngleUnit::AU_RADIAN)
            return degrees * fDeg2Rad;

        return degrees;
    }

    float sin(Radian const& rad)
    {
        return std::sin((float)rad);
    }

    float sin(float value)
    {
        return std::sin(value);
    }

    float cos(Radian const& rad)
    {
        return std::cos((float)rad);
    }

    float cos(float value)
    {
        return std::cos(value);
    }

    float tan(Radian const& rad)
    {
        return std::tan((float)rad);
    }

    float tan(float value)
    {
        return std::tan(value);
    }

    Radian acos(float value)
    {
        if (-1.0 < value)
        {
            if (value < 1.0)
                return Radian(std::acos(value));

            return Radian(0.0);
        }

        return Radian(PI);
    }
    //-----------------------------------------------------------------------
    Radian asin(float value)
    {
        if (-1.0 < value)
        {
            if (value < 1.0)
                return Radian(std::asin(value));

            return Radian(HALF_PI);
        }

        return Radian(-HALF_PI);
    }

    Radian atan(float value)
    {
        return Radian(std::atan(value));
    }

    Radian atan2(float y_v, float x_v)
    {
        return Radian(std::atan2(y_v, x_v));
    }

    Mat4
    makeViewMatrix(Vec3 const& position, Quaternion const& orientation, Mat4 const* reflect_matrix)
    {
        Mat4 viewMatrix;

        // View matrix is:
        //
        //  [ Lx  Uy  Dz  Tx  ]
        //  [ Lx  Uy  Dz  Ty  ]
        //  [ Lx  Uy  Dz  Tz  ]
        //  [ 0   0   0   1   ]
        //
        // Where T = -(Transposed(Rot) * Pos)

        // This is most efficiently done using 3x3 Matrices
        Mat3 rot;
        orientation.toRotationMatrix(rot);

        // Make the translation relative to new axes
        Mat3 rotT = rot.transpose();
        Vec3   trans = -rotT * position;

        // Make final matrix
        viewMatrix = Mat4::IDENTITY;
        viewMatrix.setMat3(rotT); // fills upper 3x3
        viewMatrix[0][3] = trans.x;
        viewMatrix[1][3] = trans.y;
        viewMatrix[2][3] = trans.z;

        // Deal with reflections
        if (reflect_matrix)
        {
            viewMatrix = viewMatrix * (*reflect_matrix);
        }

        return viewMatrix;
    }

    Mat4 makeLookAtMatrix(Vec3 const& eye_position, Vec3 const& target_position, Vec3 const& up_dir)
    {
        Vec3 const& up = up_dir.normalisedCopy();

        Vec3 f = (target_position - eye_position).normalisedCopy();
        Vec3 s = f.crossProduct(up).normalisedCopy();
        Vec3 u = s.crossProduct(f);

        Mat4 view_mat = Mat4::IDENTITY;

        view_mat[0][0] = s.x;
        view_mat[0][1] = s.y;
        view_mat[0][2] = s.z;
        view_mat[0][3] = -s.dotProduct(eye_position);
        view_mat[1][0] = u.x;
        view_mat[1][1] = u.y;
        view_mat[1][2] = u.z;
        view_mat[1][3] = -u.dotProduct(eye_position);
        view_mat[2][0] = -f.x;
        view_mat[2][1] = -f.y;
        view_mat[2][2] = -f.z;
        view_mat[2][3] = f.dotProduct(eye_position);
        return view_mat;
    }

    Mat4 makePerspectiveMatrix(Radian fovy, float aspect, float znear, float zfar)
    {
        float tan_half_fovy = tan(fovy / 2.f);

        Mat4 ret = Mat4::ZERO;
        ret[0][0] = 1.f / (aspect * tan_half_fovy);
        ret[1][1] = 1.f / tan_half_fovy;
        ret[2][2] = zfar / (znear - zfar);
        ret[3][2] = -1.f;
        ret[2][3] = -(zfar * znear) / (zfar - znear);

        return ret;
    }

    Mat4
    makeOrthographicProjectionMatrix(float left, float right, float bottom, float top, float znear, float zfar)
    {
        float inv_width = 1.0f / (right - left);
        float inv_height = 1.0f / (top - bottom);
        float inv_distance = 1.0f / (zfar - znear);

        float A = 2 * inv_width;
        float B = 2 * inv_height;
        float C = -(right + left) * inv_width;
        float D = -(top + bottom) * inv_height;
        float q = -2 * inv_distance;
        float qn = -(zfar + znear) * inv_distance;

        // NB: This creates 'uniform' orthographic projection matrix,
        // which depth range [-1,1], right-handed rules
        //
        // [ A   0   0   C  ]
        // [ 0   B   0   D  ]
        // [ 0   0   q   qn ]
        // [ 0   0   0   1  ]
        //
        // A = 2 * / (right - left)
        // B = 2 * / (top - bottom)
        // C = - (right + left) / (right - left)
        // D = - (top + bottom) / (top - bottom)
        // q = - 2 / (far - near)
        // qn = - (far + near) / (far - near)

        Mat4 proj_matrix = Mat4::ZERO;
        proj_matrix[0][0] = A;
        proj_matrix[0][3] = C;
        proj_matrix[1][1] = B;
        proj_matrix[1][3] = D;
        proj_matrix[2][2] = q;
        proj_matrix[2][3] = qn;
        proj_matrix[3][3] = 1;

        return proj_matrix;
    }

    Mat4
    makeOrthographicProjectionMatrix01(float left, float right, float bottom, float top, float znear, float zfar)
    {
        float inv_width = 1.0f / (right - left);
        float inv_height = 1.0f / (top - bottom);
        float inv_distance = 1.0f / (zfar - znear);

        float A = 2 * inv_width;
        float B = 2 * inv_height;
        float C = -(right + left) * inv_width;
        float D = -(top + bottom) * inv_height;
        float q = -1 * inv_distance;
        float qn = -znear * inv_distance;

        // NB: This creates 'uniform' orthographic projection matrix,
        // which depth range [-1,1], right-handed rules
        //
        // [ A   0   0   C  ]
        // [ 0   B   0   D  ]
        // [ 0   0   q   qn ]
        // [ 0   0   0   1  ]
        //
        // A = 2 * / (right - left)
        // B = 2 * / (top - bottom)
        // C = - (right + left) / (right - left)
        // D = - (top + bottom) / (top - bottom)
        // q = - 1 / (far - near)
        // qn = - near / (far - near)

        Mat4 proj_matrix = Mat4::ZERO;
        proj_matrix[0][0] = A;
        proj_matrix[0][3] = C;
        proj_matrix[1][1] = B;
        proj_matrix[1][3] = D;
        proj_matrix[2][2] = q;
        proj_matrix[2][3] = qn;
        proj_matrix[3][3] = 1;

        return proj_matrix;
    }
} // namespace Math
} // namespace MarsEngine
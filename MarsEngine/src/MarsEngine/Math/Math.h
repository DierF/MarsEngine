#pragma once

#include <algorithm>
#include <limits>
#include <cmath>

namespace MarsEngine
{
namespace Math
{
    class Radian;
    class Degree;
    class Vec3;
    class Mat4;
    class Quaternion;

    /** Wrapper class which identifies a value as the currently default angle
        type, as defined by Math::setAngleUnit.
    @remarks
        Angle values will be automatically converted between radians and degrees,
        as appropriate.
    */
    class Angle
    {
        float m_angle;

    public:
        explicit Angle(float angle) : m_angle(angle) {}
        Angle() { m_angle = 0; }

        explicit operator Radian() const;
        explicit operator Degree() const;
    };

    enum class AngleUnit
    {
        AU_DEGREE,
        AU_RADIAN
    };

    // angle units used by the api
    static AngleUnit s_AngleUnit;

    float const Math_POS_INFINITY = std::numeric_limits<float>::infinity();
    float const Math_NEG_INFINITY = -std::numeric_limits<float>::infinity();
    float const Math_PI = 3.14159265358979323846264338327950288f;
    float const Math_ONE_OVER_PI = 1.0f / Math_PI;
    float const Math_TWO_PI = 2.0f * Math_PI;
    float const Math_HALF_PI = 0.5f * Math_PI;
    float const Math_fDeg2Rad = Math_PI / 180.0f;
    float const Math_fRad2Deg = 180.0f / Math_PI;
    float const Math_LOG2 = log(2.0f);
    float const Math_EPSILON = 1e-6f;

    float const Float_EPSILON = FLT_EPSILON;
    float const Double_EPSILON = DBL_EPSILON;

    float abs(float value);
    bool  isNan(float f);
    float sqr(float value);
    float sqrt(float fValue);
    float invSqrt(float value);
    bool  realEqual(float a, float b, float tolerance = std::numeric_limits<float>::epsilon());
    float clamp(float v, float min, float max);
    float getMaxElement(float x, float y, float z);

    float degreesToRadians(float degrees);
    float radiansToDegrees(float radians);
    float angleUnitsToRadians(float units);
    float radiansToAngleUnits(float radians);
    float angleUnitsToDegrees(float units);
    float degreesToAngleUnits(float degrees);

    float  sin(const Radian& rad);
    float  sin(float value);
    float  cos(const Radian& rad);
    float  cos(float value);
    float  tan(const Radian& rad);
    float  tan(float value);
    Radian acos(float value);
    Radian asin(float value);
    Radian atan(float value);
    Radian atan2(float y_v, float x_v);

    template<class T>
    constexpr T max(const T A, const T B)
    {
        return std::max(A, B);
    }

    template<class T>
    constexpr T min(const T A, const T B)
    {
        return std::min(A, B);
    }

    template<class T>
    constexpr T max3(const T A, const T B, const T C)
    {
        return std::max({ A, B, C });
    }

    template<class T>
    constexpr T min3(const T A, const T B, const T C)
    {
        return std::min({ A, B, C });
    }

    Mat4
    makeViewMatrix(Vec3 const& position, Quaternion const& orientation, Mat4 const* reflect_matrix = nullptr);

    Mat4
    makeLookAtMatrix(Vec3 const& eye_position, Vec3 const& target_position, Vec3 const& up_dir);

    Mat4 makePerspectiveMatrix(Radian fovy, float aspect, float znear, float zfar);

    Mat4
    makeOrthographicProjectionMatrix(float left, float right, float bottom, float top, float znear, float zfar);

    Mat4
    makeOrthographicProjectionMatrix01(float left, float right, float bottom, float top, float znear, float zfar);
} // namespace Math
} // namespace MarsEngine
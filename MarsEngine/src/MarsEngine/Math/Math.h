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
    static AngleUnit s_AngleUnit = AngleUnit::AU_DEGREE;

    static const float Math_POS_INFINITY = std::numeric_limits<float>::infinity();
    static const float Math_NEG_INFINITY = -std::numeric_limits<float>::infinity();
    static const float Math_PI = 3.14159265358979323846264338327950288f;
    static const float Math_ONE_OVER_PI = 1.0f / Math_PI;
    static const float Math_TWO_PI = 2.0f * Math_PI;
    static const float Math_HALF_PI = 0.5f * Math_PI;
    static const float Math_fDeg2Rad = Math_PI / 180.0f;
    static const float Math_fRad2Deg = 180.0f / Math_PI;
    static const float Math_LOG2 = log(2.0f);
    static const float Math_EPSILON = 1e-6f;

    static const float Float_EPSILON = FLT_EPSILON;
    static const float Double_EPSILON = DBL_EPSILON;

    static float abs(float value) { return std::fabs(value); }
    static bool  isNan(float f) { return std::isnan(f); }
    static float sqr(float value) { return value * value; }
    static float sqrt(float fValue) { return std::sqrt(fValue); }
    static float invSqrt(float value) { return 1.f / sqrt(value); }
    static bool  realEqual(float a, float b, float tolerance = std::numeric_limits<float>::epsilon());
    static float clamp(float v, float min, float max) { return std::clamp(v, min, max); }
    static float getMaxElement(float x, float y, float z) { return std::max({ x, y, z }); }

    static float degreesToRadians(float degrees);
    static float radiansToDegrees(float radians);
    static float angleUnitsToRadians(float units);
    static float radiansToAngleUnits(float radians);
    static float angleUnitsToDegrees(float units);
    static float degreesToAngleUnits(float degrees);

    static float  sin(const Radian& rad);
    static float  sin(float value) { return std::sin(value); }
    static float  cos(const Radian& rad);
    static float  cos(float value) { return std::cos(value); }
    static float  tan(const Radian& rad);
    static float  tan(float value) { return std::tan(value); }
    static Radian acos(float value);
    static Radian asin(float value);
    static Radian atan(float value);
    static Radian atan2(float y_v, float x_v);

    template<class T>
    static constexpr T max(const T A, const T B)
    {
        return std::max(A, B);
    }

    template<class T>
    static constexpr T min(const T A, const T B)
    {
        return std::min(A, B);
    }

    template<class T>
    static constexpr T max3(const T A, const T B, const T C)
    {
        return std::max({ A, B, C });
    }

    template<class T>
    static constexpr T min3(const T A, const T B, const T C)
    {
        return std::min({ A, B, C });
    }

    static Mat4
        makeViewMatrix(const Vec3& position, const Quaternion& orientation, const Mat4* reflect_matrix = nullptr);

    static Mat4
        makeLookAtMatrix(const Vec3& eye_position, const Vec3& target_position, const Vec3& up_dir);

    static Mat4 makePerspectiveMatrix(Radian fovy, float aspect, float znear, float zfar);

    static Mat4
        makeOrthographicProjectionMatrix(float left, float right, float bottom, float top, float znear, float zfar);

    static Mat4
        makeOrthographicProjectionMatrix01(float left, float right, float bottom, float top, float znear, float zfar);
} // namespace Math
} // namespace MarsEngine
#pragma once

#include "Math.h"
#include "Vector3.h"

namespace MarsEngine
{
namespace Math
{
    //REFLECTION_TYPE(Vec4)
        //CLASS(Vec4, Fields)
    class Vec4
    {
        //REFLECTION_BODY(Vec4);

    public:
        float x{ 0.f }, y{ 0.f }, z{ 0.f }, w{ 0.f };

    public:
        Vec4() = default;
        Vec4(float x_, float y_, float z_, float w_) : x{ x_ }, y{ y_ }, z{ z_ }, w{ w_ } {}
        Vec4(Vec3 const& v3, float w_) : x{ v3.x }, y{ v3.y }, z{ v3.z }, w{ w_ } {}

        explicit Vec4(float coords[4]) : x{ coords[0] }, y{ coords[1] }, z{ coords[2] }, w{ coords[3] } {}

        float operator[](size_t i) const
        {
            assert(i < 4);
            return *(&x + i);
        }

        float& operator[](size_t i)
        {
            assert(i < 4);
            return *(&x + i);
        }

        /// Pointer accessor for direct copying
        float* ptr() { return &x; }
        /// Pointer accessor for direct copying
        float const* ptr() const { return &x; }

        Vec4& operator=(float scalar)
        {
            x = scalar;
            y = scalar;
            z = scalar;
            w = scalar;
            return *this;
        }

        bool operator==(Vec4 const& rhs) const { return (x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w); }

        bool operator!=(Vec4 const& rhs) const { return !(rhs == *this); }

        Vec4 operator+(Vec4 const& rhs) const { return Vec4(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w); }
        Vec4 operator-(Vec4 const& rhs) const { return Vec4(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w); }
        Vec4 operator*(float scalar) const { return Vec4(x * scalar, y * scalar, z * scalar, w * scalar); }
        Vec4 operator*(Vec4 const& rhs) const { return Vec4(rhs.x * x, rhs.y * y, rhs.z * z, rhs.w * w); }
        Vec4 operator/(float scalar) const
        {
            assert(scalar != 0.0);
            return Vec4(x / scalar, y / scalar, z / scalar, w / scalar);
        }
        Vec4 operator/(Vec4 const& rhs) const
        {
            assert(rhs.x != 0 && rhs.y != 0 && rhs.z != 0 && rhs.w != 0);
            return Vec4(x / rhs.x, y / rhs.y, z / rhs.z, w / rhs.w);
        }

        Vec4 const& operator+() const { return *this; }

        Vec4 operator-() const { return Vec4(-x, -y, -z, -w); }

        friend Vec4 operator*(float scalar, Vec4 const& rhs)
        {
            return Vec4(scalar * rhs.x, scalar * rhs.y, scalar * rhs.z, scalar * rhs.w);
        }

        friend Vec4 operator/(float scalar, Vec4 const& rhs)
        {
            assert(rhs.x != 0 && rhs.y != 0 && rhs.z != 0 && rhs.w != 0);
            return Vec4(scalar / rhs.x, scalar / rhs.y, scalar / rhs.z, scalar / rhs.w);
        }

        friend Vec4 operator+(Vec4 const& lhs, float rhs)
        {
            return Vec4(lhs.x + rhs, lhs.y + rhs, lhs.z + rhs, lhs.w + rhs);
        }

        friend Vec4 operator+(float lhs, Vec4 const& rhs)
        {
            return Vec4(lhs + rhs.x, lhs + rhs.y, lhs + rhs.z, lhs + rhs.w);
        }

        friend Vec4 operator-(Vec4 const& lhs, float rhs)
        {
            return Vec4(lhs.x - rhs, lhs.y - rhs, lhs.z - rhs, lhs.w - rhs);
        }

        friend Vec4 operator-(float lhs, Vec4 const& rhs)
        {
            return Vec4(lhs - rhs.x, lhs - rhs.y, lhs - rhs.z, lhs - rhs.w);
        }

        // arithmetic updates
        Vec4& operator+=(Vec4 const& rhs)
        {
            x += rhs.x;
            y += rhs.y;
            z += rhs.z;
            w += rhs.w;
            return *this;
        }

        Vec4& operator-=(Vec4 const& rhs)
        {
            x -= rhs.x;
            y -= rhs.y;
            z -= rhs.z;
            w -= rhs.w;
            return *this;
        }

        Vec4& operator*=(float scalar)
        {
            x *= scalar;
            y *= scalar;
            z *= scalar;
            w *= scalar;
            return *this;
        }

        Vec4& operator+=(float scalar)
        {
            x += scalar;
            y += scalar;
            z += scalar;
            w += scalar;
            return *this;
        }

        Vec4& operator-=(float scalar)
        {
            x -= scalar;
            y -= scalar;
            z -= scalar;
            w -= scalar;
            return *this;
        }

        Vec4& operator*=(Vec4 const& rhs)
        {
            x *= rhs.x;
            y *= rhs.y;
            z *= rhs.z;
            w *= rhs.w;
            return *this;
        }

        Vec4& operator/=(float scalar)
        {
            assert(scalar != 0.0);

            x /= scalar;
            y /= scalar;
            z /= scalar;
            w /= scalar;
            return *this;
        }

        Vec4& operator/=(Vec4 const& rhs)
        {
            assert(rhs.x != 0 && rhs.y != 0 && rhs.z != 0);
            x /= rhs.x;
            y /= rhs.y;
            z /= rhs.z;
            w /= rhs.w;
            return *this;
        }

        /** Calculates the dot (scalar) product of this vector with another.
        @param
        vec Vector with which to calculate the dot product (together
        with this one).
        @returns
        A float representing the dot product value.
        */
        float dotProduct(Vec4 const& vec) const { return x * vec.x + y * vec.y + z * vec.z + w * vec.w; }

        /// Check whether this vector contains valid values
        bool isNaN() const { return isNan(x) || isNan(y) || isNan(z) || isNan(w); }

        // special
        static const Vec4 ZERO;
        static const Vec4 UNIT_SCALE;
    };
} // namespace Math
} // namespace MarsEngine
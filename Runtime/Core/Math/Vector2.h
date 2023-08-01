#pragma once

#include "Runtime/Core/meta/Reflection/Reflection.h"
#include "Runtime/Core/Math/Math.h"

#include <cassert>
#include <cmath>

namespace MarsEngine
{
    REFLECTION_TYPE(Vec2)
    CLASS(Vec2, Fields)
    {
        REFLECTION_BODY(Vec2);

    public:
        float x{ 0.f }, y{ 0.f };

    public:
        Vec2() = default;

        Vec2(float x_, float y_) : x(x_), y(y_) {}

        explicit Vec2(float scaler) : x(scaler), y(scaler) {}

        explicit Vec2(const float v[2]) : x(v[0]), y(v[1]) {}

        explicit Vec2(float* const r) : x(r[0]), y(r[1]) {}

        float* ptr() { return &x; }

        const float* ptr() const { return &x; }

        float operator[](size_t i) const
        {
            assert(i < 2);
            return (i == 0 ? x : y);
        }

        float& operator[](size_t i)
        {
            assert(i < 2);
            return (i == 0 ? x : y);
        }

        bool operator==(const Vec2 & rhs) const { return (x == rhs.x && y == rhs.y); }

        bool operator!=(const Vec2 & rhs) const { return (x != rhs.x || y != rhs.y); }

        // arithmetic operations
        Vec2 operator+(const Vec2 & rhs) const { return Vec2(x + rhs.x, y + rhs.y); }

        Vec2 operator-(const Vec2 & rhs) const { return Vec2(x - rhs.x, y - rhs.y); }

        Vec2 operator*(float scalar) const { return Vec2(x * scalar, y * scalar); }

        Vec2 operator*(const Vec2 & rhs) const { return Vec2(x * rhs.x, y * rhs.y); }

        Vec2 operator/(float scale) const
        {
            assert(scale != 0.0);

            float inv = 1.0f / scale;
            return Vec2(x * inv, y * inv);
        }

        Vec2 operator/(const Vec2 & rhs) const { return Vec2(x / rhs.x, y / rhs.y); }

        const Vec2& operator+() const { return *this; }

        Vec2 operator-() const { return Vec2(-x, -y); }

        // overloaded operators to help Vec2
        friend Vec2 operator*(float scalar, const Vec2 & rhs) { return Vec2(scalar * rhs.x, scalar * rhs.y); }

        friend Vec2 operator/(float fScalar, const Vec2 & rhs)
        {
            return Vec2(fScalar / rhs.x, fScalar / rhs.y);
        }

        friend Vec2 operator+(const Vec2 & lhs, float rhs) { return Vec2(lhs.x + rhs, lhs.y + rhs); }

        friend Vec2 operator+(float lhs, const Vec2 & rhs) { return Vec2(lhs + rhs.x, lhs + rhs.y); }

        friend Vec2 operator-(const Vec2 & lhs, float rhs) { return Vec2(lhs.x - rhs, lhs.y - rhs); }

        friend Vec2 operator-(float lhs, const Vec2 & rhs) { return Vec2(lhs - rhs.x, lhs - rhs.y); }

        // arithmetic updates
        Vec2& operator+=(const Vec2 & rhs)
        {
            x += rhs.x;
            y += rhs.y;

            return *this;
        }

        Vec2& operator+=(float scalar)
        {
            x += scalar;
            y += scalar;

            return *this;
        }

        Vec2& operator-=(const Vec2 & rhs)
        {
            x -= rhs.x;
            y -= rhs.y;

            return *this;
        }

        Vec2& operator-=(float scalar)
        {
            x -= scalar;
            y -= scalar;

            return *this;
        }

        Vec2& operator*=(float scalar)
        {
            x *= scalar;
            y *= scalar;

            return *this;
        }

        Vec2& operator*=(const Vec2 & rhs)
        {
            x *= rhs.x;
            y *= rhs.y;

            return *this;
        }

        Vec2& operator/=(float scalar)
        {
            assert(scalar != 0.0);

            float inv = 1.0f / scalar;

            x *= inv;
            y *= inv;

            return *this;
        }

        Vec2& operator/=(const Vec2 & rhs)
        {
            x /= rhs.x;
            y /= rhs.y;

            return *this;
        }

        /** Returns the length (magnitude) of the vector.
        @warning
        This operation requires a square root and is expensive in
        terms of CPU operations. If you don't need to know the exact
        length (e.g. for just comparing lengths) use squaredLength()
        instead.
        */
        float length() const { return std::hypot(x, y); }

        /** Returns the square of the length(magnitude) of the vector.
        @remarks
        This  method is for efficiency - calculating the actual
        length of a vector requires a square root, which is expensive
        in terms of the operations required. This method returns the
        square of the length of the vector, i.e. the same as the
        length but before the square root is taken. Use this if you
        want to find the longest / shortest vector without incurring
        the square root.
        */
        float squaredLength() const { return x * x + y * y; }

        /** Returns the distance to another vector.
        @warning
        This operation requires a square root and is expensive in
        terms of CPU operations. If you don't need to know the exact
        distance (e.g. for just comparing distances) use squaredDistance()
        instead.
        */
        float distance(const Vec2 & rhs) const { return (*this - rhs).length(); }

        /** Returns the square of the distance to another vector.
        @remarks
        This method is for efficiency - calculating the actual
        distance to another vector requires a square root, which is
        expensive in terms of the operations required. This method
        returns the square of the distance to another vector, i.e.
        the same as the distance but before the square root is taken.
        Use this if you want to find the longest / shortest distance
        without incurring the square root.
        */
        float squaredDistance(const Vec2 & rhs) const { return (*this - rhs).squaredLength(); }

        /** Calculates the dot (scalar) product of this vector with another.
        @remarks
        The dot product can be used to calculate the angle between 2
        vectors. If both are unit vectors, the dot product is the
        cosine of the angle; otherwise the dot product must be
        divided by the product of the lengths of both vectors to get
        the cosine of the angle. This result can further be used to
        calculate the distance of a point from a plane.
        @param
        vec Vector with which to calculate the dot product (together
        with this one).
        @returns
        A float representing the dot product value.
        */
        float dotProduct(const Vec2 & vec) const { return x * vec.x + y * vec.y; }

        /** Normalizes the vector.
        @remarks
        This method normalizes the vector such that it's
        length / magnitude is 1. The result is called a unit vector.
        @note
        This function will not crash for zero-sized vectors, but there
        will be no changes made to their components.
        @returns The previous length of the vector.
        */

        float normalise()
        {
            float lengh = std::hypot(x, y);

            if (lengh > 0.0f)
            {
                float inv_length = 1.0f / lengh;
                x *= inv_length;
                y *= inv_length;
            }

            return lengh;
        }

        float getX() const { return x; }
        float getY() const { return y; }

        void setX(float value) { x = value; }
        void setY(float value) { y = value; }

        /** Returns a vector at a point half way between this and the passed
        in vector.
        */
        Vec2 midPoint(const Vec2 & vec) const { return Vec2((x + vec.x) * 0.5f, (y + vec.y) * 0.5f); }

        /** Returns true if the vector's scalar components are all greater
        that the ones of the vector it is compared against.
        */
        bool operator<(const Vec2 & rhs) const { return x < rhs.x && y < rhs.y; }

        /** Returns true if the vector's scalar components are all smaller
        that the ones of the vector it is compared against.
        */
        bool operator>(const Vec2 & rhs) const { return x > rhs.x && y > rhs.y; }

        /** Sets this vector's components to the minimum of its own and the
        ones of the passed in vector.
        @remarks
        'Minimum' in this case means the combination of the lowest
        value of x, y and z from both vectors. Lowest is taken just
        numerically, not magnitude, so -1 < 0.
        */
        void makeFloor(const Vec2 & cmp)
        {
            if (cmp.x < x)
                x = cmp.x;
            if (cmp.y < y)
                y = cmp.y;
        }

        /** Sets this vector's components to the maximum of its own and the
        ones of the passed in vector.
        @remarks
        'Maximum' in this case means the combination of the highest
        value of x, y and z from both vectors. Highest is taken just
        numerically, not magnitude, so 1 > -3.
        */
        void makeCeil(const Vec2 & cmp)
        {
            if (cmp.x > x)
                x = cmp.x;
            if (cmp.y > y)
                y = cmp.y;
        }

        /** Generates a vector perpendicular to this vector (eg an 'up' vector).
        @remarks
        This method will return a vector which is perpendicular to this
        vector. There are an infinite number of possibilities but this
        method will guarantee to generate one of them. If you need more
        control you should use the Quaternion class.
        */
        Vec2 perpendicular(void) const { return Vec2(-y, x); }

        /** Calculates the 2 dimensional cross-product of 2 vectors, which results
        in a single floating point value which is 2 times the area of the triangle.
        */

        float crossProduct(const Vec2 & rhs) const { return x * rhs.y - y * rhs.x; }

        /** Returns true if this vector is zero length. */
        bool isZeroLength(void) const
        {
            float sqlen = (x * x) + (y * y);
            return (sqlen < (Math::Float_EPSILON * Math::Float_EPSILON));
        }

        /** As normalise, except that this vector is unaffected and the
        normalised vector is returned as a copy. */
        Vec2 normalisedCopy(void) const
        {
            Vec2 ret = *this;
            ret.normalise();
            return ret;
        }

        /** Calculates a reflection vector to the plane with the given normal .
        @remarks NB assumes 'this' is pointing AWAY FROM the plane, invert if it is not.
        */
        Vec2 reflect(const Vec2 & normal) const
        {
            return Vec2(*this - (2 * this->dotProduct(normal) * normal));
        }

        /// Check whether this vector contains valid values
        bool isNaN() const { return Math::isNan(x) || Math::isNan(y); }

        static Vec2 lerp(const Vec2 & lhs, const Vec2 & rhs, float alpha) { return lhs + alpha * (rhs - lhs); }

        // special points
        static const Vec2 ZERO;
        static const Vec2 UNIT_X;
        static const Vec2 UNIT_Y;
        static const Vec2 NEGATIVE_UNIT_X;
        static const Vec2 NEGATIVE_UNIT_Y;
        static const Vec2 UNIT_SCALE;
    };
} // namespace MarsEngine
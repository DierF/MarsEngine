#pragma once

#include "Runtime/Core/meta/Reflection/Reflection.h"
#include "Runtime/Core/Math/Math.h"
#include "Runtime/Core/Math/Radian.h"
#include "Runtime/Core/Math/Quaternion.h"

#include <cassert>

namespace MarsEngine
{
    REFLECTION_TYPE(Vec3)
    CLASS(Vec3, Fields)
    {
        REFLECTION_BODY(Vec3);

    public:
        float x{ 0.f };
        float y{ 0.f };
        float z{ 0.f };

    public:
        Vec3() = default;
        Vec3(float x_, float y_, float z_) : x{ x_ }, y{ y_ }, z{ z_ } {}

        explicit Vec3(const float coords[3]) : x{ coords[0] }, y{ coords[1] }, z{ coords[2] } {}

        float operator[](size_t i) const
        {
            assert(i < 3);
            return *(&x + i);
        }

        float& operator[](size_t i)
        {
            assert(i < 3);
            return *(&x + i);
        }
        /// Pointer accessor for direct copying
        float* ptr() { return &x; }
        /// Pointer accessor for direct copying
        const float* ptr() const { return &x; }

        bool operator==(Vec3 const& rhs) const { return (x == rhs.x && y == rhs.y && z == rhs.z); }

        bool operator!=(Vec3 const& rhs) const { return x != rhs.x || y != rhs.y || z != rhs.z; }

        // arithmetic operations
        Vec3 operator+(Vec3 const& rhs) const { return Vec3(x + rhs.x, y + rhs.y, z + rhs.z); }

        Vec3 operator-(Vec3 const& rhs) const { return Vec3(x - rhs.x, y - rhs.y, z - rhs.z); }

        Vec3 operator*(float scalar) const { return Vec3(x * scalar, y * scalar, z * scalar); }

        Vec3 operator*(Vec3 const& rhs) const { return Vec3(x * rhs.x, y * rhs.y, z * rhs.z); }

        Vec3 operator/(float scalar) const
        {
            assert(scalar != 0.0);
            return Vec3(x / scalar, y / scalar, z / scalar);
        }

        Vec3 operator/(Vec3 const& rhs) const
        {
            assert((rhs.x != 0 && rhs.y != 0 && rhs.z != 0));
            return Vec3(x / rhs.x, y / rhs.y, z / rhs.z);
        }

        Vec3 const& operator+() const { return *this; }

        Vec3 operator-() const { return Vec3(-x, -y, -z); }

        // overloaded operators to help Vec3
        friend Vec3 operator*(float scalar, Vec3 const& rhs)
        {
            return Vec3(scalar * rhs.x, scalar * rhs.y, scalar * rhs.z);
        }

        friend Vec3 operator/(float scalar, Vec3 const& rhs)
        {
            assert(rhs.x != 0 && rhs.y != 0 && rhs.z != 0);
            return Vec3(scalar / rhs.x, scalar / rhs.y, scalar / rhs.z);
        }

        friend Vec3 operator+(Vec3 const& lhs, float rhs)
        {
            return Vec3(lhs.x + rhs, lhs.y + rhs, lhs.z + rhs);
        }

        friend Vec3 operator+(float lhs, Vec3 const& rhs)
        {
            return Vec3(lhs + rhs.x, lhs + rhs.y, lhs + rhs.z);
        }

        friend Vec3 operator-(Vec3 const& lhs, float rhs)
        {
            return Vec3(lhs.x - rhs, lhs.y - rhs, lhs.z - rhs);
        }

        friend Vec3 operator-(float lhs, Vec3 const& rhs)
        {
            return Vec3(lhs - rhs.x, lhs - rhs.y, lhs - rhs.z);
        }

        // arithmetic updates
        Vec3& operator+=(Vec3 const& rhs)
        {
            x += rhs.x;
            y += rhs.y;
            z += rhs.z;
            return *this;
        }

        Vec3& operator+=(float scalar)
        {
            x += scalar;
            y += scalar;
            z += scalar;
            return *this;
        }

        Vec3& operator-=(Vec3 const& rhs)
        {
            x -= rhs.x;
            y -= rhs.y;
            z -= rhs.z;
            return *this;
        }

        Vec3& operator-=(float scalar)
        {
            x -= scalar;
            y -= scalar;
            z -= scalar;
            return *this;
        }

        Vec3& operator*=(float scalar)
        {
            x *= scalar;
            y *= scalar;
            z *= scalar;
            return *this;
        }

        Vec3& operator*=(Vec3 const& rhs)
        {
            x *= rhs.x;
            y *= rhs.y;
            z *= rhs.z;
            return *this;
        }

        Vec3& operator/=(float scalar)
        {
            assert(scalar != 0.0);
            x /= scalar;
            y /= scalar;
            z /= scalar;
            return *this;
        }

        Vec3& operator/=(Vec3 const& rhs)
        {
            assert(rhs.x != 0 && rhs.y != 0 && rhs.z != 0);
            x /= rhs.x;
            y /= rhs.y;
            z /= rhs.z;
            return *this;
        }

        /** Returns the length (magnitude) of the vector.
        @warning
        This operation requires a square root and is expensive in
        terms of CPU operations. If you don't need to know the exact
        length (e.g. for just comparing lengths) use squaredLength()
        instead.
        */

        float length() const { return std::hypot(x, y, z); }

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
        float squaredLength() const { return x * x + y * y + z * z; }

        /** Returns the distance to another vector.
        @warning
        This operation requires a square root and is expensive in
        terms of CPU operations. If you don't need to know the exact
        distance (e.g. for just comparing distances) use squaredDistance()
        instead.
        */

        float distance(Vec3 const& rhs) const { return (*this - rhs).length(); }

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

        float squaredDistance(Vec3 const& rhs) const { return (*this - rhs).squaredLength(); }

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

        float dotProduct(Vec3 const& vec) const { return x * vec.x + y * vec.y + z * vec.z; }

        /** Normalizes the vector.
        @remarks
        This method normalizes the vector such that it's
        length / magnitude is 1. The result is called a unit vector.
        @note
        This function will not crash for zero-sized vectors, but there
        will be no changes made to their components.
        @returns The previous length of the vector.
        */

        void normalise()
        {
            float length = std::hypot(x, y, z);
            if (length == 0.f)
                return;

            float inv_lengh = 1.0f / length;
            x *= inv_lengh;
            y *= inv_lengh;
            z *= inv_lengh;
        }

        /** Calculates the cross-product of 2 vectors, i.e. the vector that
        lies perpendicular to them both.
        @remarks
        The cross-product is normally used to calculate the normal
        vector of a plane, by calculating the cross-product of 2
        non-equivalent vectors which lie on the plane (e.g. 2 edges
        of a triangle).
        @param
        vec Vector which, together with this one, will be used to
        calculate the cross-product.
        @returns
        A vector which is the result of the cross-product. This
        vector will <b>NOT</b> be normalised, to maximize efficiency
        - call Vec3::normalise on the result if you wish this to
        be done. As for which side the resultant vector will be on, the
        returned vector will be on the side from which the arc from 'this'
        to rkVector is anticlockwise, e.g. UNIT_Y.crossProduct(UNIT_Z)
        = UNIT_X, whilst UNIT_Z.crossProduct(UNIT_Y) = -UNIT_X.
        This is because CHAOS uses a right-handed coordinate system.
        @par
        For a clearer explanation, look a the left and the bottom edges
        of your monitor's screen. Assume that the first vector is the
        left edge and the second vector is the bottom edge, both of
        them starting from the lower-left corner of the screen. The
        resulting vector is going to be perpendicular to both of them
        and will go <i>inside</i> the screen, towards the cathode tube
        (assuming you're using a CRT monitor, of course).
        */

        Vec3 crossProduct(Vec3 const& rhs) const
        {
            return Vec3(y * rhs.z - z * rhs.y, z * rhs.x - x * rhs.z, x * rhs.y - y * rhs.x);
        }

        /** Sets this vector's components to the minimum of its own and the
        ones of the passed in vector.
        @remarks
        'Minimum' in this case means the combination of the lowest
        value of x, y and z from both vectors. Lowest is taken just
        numerically, not magnitude, so -1 < 0.
        */
        void makeFloor(Vec3 const& cmp)
        {
            if (cmp.x < x)
                x = cmp.x;
            if (cmp.y < y)
                y = cmp.y;
            if (cmp.z < z)
                z = cmp.z;
        }

        /** Sets this vector's components to the maximum of its own and the
        ones of the passed in vector.
        @remarks
        'Maximum' in this case means the combination of the highest
        value of x, y and z from both vectors. Highest is taken just
        numerically, not magnitude, so 1 > -3.
        */
        void makeCeil(Vec3 const& cmp)
        {
            if (cmp.x > x)
                x = cmp.x;
            if (cmp.y > y)
                y = cmp.y;
            if (cmp.z > z)
                z = cmp.z;
        }

        /** Gets the angle between 2 vectors.
        @remarks
        Vectors do not have to be unit-length but must represent directions.
        */

        Radian angleBetween(Vec3 const& dest) const
        {
            float len_product = length() * dest.length();

            // Divide by zero check
            if (len_product < 1e-6f)
                len_product = 1e-6f;

            float f = dotProduct(dest) / len_product;

            f = Math::clamp(f, (float)-1.0, (float)1.0);
            return Math::acos(f);
        }
        /** Gets the shortest arc quaternion to rotate this vector to the destination
        vector.
        @remarks
        If you call this with a dest vector that is close to the inverse
        of this vector, we will rotate 180 degrees around the 'fallbackAxis'
        (if specified, or a generated axis if not) since in this case
        ANY axis of rotation is valid.
        */

        Quaternion getRotationTo(Vec3 const& dest, Vec3 const& fallback_axis = Vec3::ZERO) const
        {
            // Based on Stan Melax's article in Game Programming Gems
            Quaternion q;
            // Copy, since cannot modify local
            Vec3 v0 = *this;
            Vec3 v1 = dest;
            v0.normalise();
            v1.normalise();

            float d = v0.dotProduct(v1);
            // If dot == 1, vectors are the same
            if (d >= 1.0f)
            {
                return Quaternion::IDENTITY;
            }
            if (d < (1e-6f - 1.0f))
            {
                if (fallback_axis != Vec3::ZERO)
                {
                    // rotate 180 degrees about the fall back axis
                    q.fromAngleAxis(Radian(Math::PI), fallback_axis);
                }
                else
                {
                    // Generate an axis
                    Vec3 axis = Vec3::UNIT_X.crossProduct(*this);
                    if (axis.isZeroLength()) // pick another if collinear
                        axis = Vec3::UNIT_Y.crossProduct(*this);
                    axis.normalise();
                    q.fromAngleAxis(Radian(Math::PI), axis);
                }
            }
            else
            {
                float s = Math::sqrt((1 + d) * 2);
                float invs = 1 / s;

                Vec3 c = v0.crossProduct(v1);

                q.x = c.x * invs;
                q.y = c.y * invs;
                q.z = c.z * invs;
                q.w = s * 0.5f;
                q.normalise();
            }
            return q;
        }

        /** Returns true if this vector is zero length. */
        bool isZeroLength(void) const
        {
            float sqlen = (x * x) + (y * y) + (z * z);
            return (sqlen < (1e-06 * 1e-06));
        }

        bool isZero() const { return x == 0.f && y == 0.f && z == 0.f; }

        /** As normalise, except that this vector is unaffected and the
        normalised vector is returned as a copy. */

        Vec3 normalisedCopy(void) const
        {
            Vec3 ret = *this;
            ret.normalise();
            return ret;
        }

        /** Calculates a reflection vector to the plane with the given normal .
        @remarks NB assumes 'this' is pointing AWAY FROM the plane, invert if it is not.
        */
        Vec3 reflect(Vec3 const& normal) const
        {
            return Vec3(*this - (2 * this->dotProduct(normal) * normal));
        }

        /** Calculates projection to a plane with the given normal
        @param normal The normal of given plane
        */
        Vec3 project(Vec3 const& normal) const { return Vec3(*this - (this->dotProduct(normal) * normal)); }

        Vec3 absoluteCopy() const { return Vec3(fabsf(x), fabsf(y), fabsf(z)); }

        static Vec3 lerp(Vec3 const& lhs, Vec3 const& rhs, float alpha) { return lhs + alpha * (rhs - lhs); }

        static Vec3 clamp(Vec3 const& v, Vec3 const& min, Vec3 const& max)
        {
            return Vec3(
                Math::clamp(v.x, min.x, max.x), Math::clamp(v.y, min.y, max.y), Math::clamp(v.z, min.z, max.z));
        }

        static float getMaxElement(Vec3 const& v) { return Math::getMaxElement(v.x, v.y, v.z); }
        bool         isNaN() const { return Math::isNan(x) || Math::isNan(y) || Math::isNan(z); }
        // special points
        static Vec3 const ZERO;
        static Vec3 const UNIT_X;
        static Vec3 const UNIT_Y;
        static Vec3 const UNIT_Z;
        static Vec3 const NEGATIVE_UNIT_X;
        static Vec3 const NEGATIVE_UNIT_Y;
        static Vec3 const NEGATIVE_UNIT_Z;
        static Vec3 const UNIT_SCALE;
    };
} // namespace MarsEngine
#include "pch.h"

#include "Quaternion.h"
#include "Vector3.h"
#include "Matrix3.h"
#include "Matrix4.h"
#include "Radian.h"

namespace MarsEngine
{
namespace Math
{
    Quaternion const Quaternion::ZERO(0, 0, 0, 0);
    Quaternion const Quaternion::IDENTITY(1, 0, 0, 0);

    float const Quaternion::k_epsilon = 1e-03;

    Quaternion Quaternion::operator*(Quaternion const& rhs) const
    {
        return Quaternion(w * rhs.w - x * rhs.x - y * rhs.y - z * rhs.z,
            w * rhs.x + x * rhs.w + y * rhs.z - z * rhs.y,
            w * rhs.y + y * rhs.w + z * rhs.x - x * rhs.z,
            w * rhs.z + z * rhs.w + x * rhs.y - y * rhs.x);
    }

    //-----------------------------------------------------------------------
    void Quaternion::fromRotationMatrix(Mat3 const& rotation)
    {
        // Algorithm in Ken Shoemake's article in 1987 SIGGRAPH course notes
        // article "Quaternion Calculus and Fast Animation".

        float trace = rotation[0][0] + rotation[1][1] + rotation[2][2];
        float root;

        if (trace > 0.0)
        {
            // |w| > 1/2, may as well choose w > 1/2
            root = std::sqrt(trace + 1.0f); // 2w
            w = 0.5f * root;
            root = 0.5f / root; // 1/(4w)
            x = (rotation[2][1] - rotation[1][2]) * root;
            y = (rotation[0][2] - rotation[2][0]) * root;
            z = (rotation[1][0] - rotation[0][1]) * root;
        }
        else
        {
            // |w| <= 1/2
            size_t s_iNext[3] = { 1, 2, 0 };
            size_t i = 0;
            if (rotation[1][1] > rotation[0][0])
                i = 1;
            if (rotation[2][2] > rotation[i][i])
                i = 2;
            size_t j = s_iNext[i];
            size_t k = s_iNext[j];

            root = std::sqrt(rotation[i][i] - rotation[j][j] - rotation[k][k] + 1.0f);
            float* apkQuat[3] = { &x, &y, &z };
            *apkQuat[i] = 0.5f * root;
            root = 0.5f / root;
            w = (rotation[k][j] - rotation[j][k]) * root;
            *apkQuat[j] = (rotation[j][i] + rotation[i][j]) * root;
            *apkQuat[k] = (rotation[k][i] + rotation[i][k]) * root;
        }
    }
    //-----------------------------------------------------------------------
    void Quaternion::toRotationMatrix(Mat3& kRot) const
    {
        float fTx = x + x;   // 2x
        float fTy = y + y;   // 2y
        float fTz = z + z;   // 2z
        float fTwx = fTx * w; // 2xw
        float fTwy = fTy * w; // 2yw
        float fTwz = fTz * w; // 2z2
        float fTxx = fTx * x; // 2x^2
        float fTxy = fTy * x; // 2xy
        float fTxz = fTz * x; // 2xz
        float fTyy = fTy * y; // 2y^2
        float fTyz = fTz * y; // 2yz
        float fTzz = fTz * z; // 2z^2

        kRot[0][0] = 1.0f - (fTyy + fTzz); // 1 - 2y^2 - 2z^2
        kRot[0][1] = fTxy - fTwz;          // 2xy - 2wz
        kRot[0][2] = fTxz + fTwy;          // 2xz + 2wy
        kRot[1][0] = fTxy + fTwz;          // 2xy + 2wz
        kRot[1][1] = 1.0f - (fTxx + fTzz); // 1 - 2x^2 - 2z^2
        kRot[1][2] = fTyz - fTwx;          // 2yz - 2wx
        kRot[2][0] = fTxz - fTwy;          // 2xz - 2wy
        kRot[2][1] = fTyz + fTwx;          // 2yz + 2wx
        kRot[2][2] = 1.0f - (fTxx + fTyy); // 1 - 2x^2 - 2y^2
    }

    void Quaternion::toRotationMatrix(Mat4& kRot) const
    {
        float fTx = x + x;   // 2x
        float fTy = y + y;   // 2y
        float fTz = z + z;   // 2z
        float fTwx = fTx * w; // 2xw
        float fTwy = fTy * w; // 2yw
        float fTwz = fTz * w; // 2z2
        float fTxx = fTx * x; // 2x^2
        float fTxy = fTy * x; // 2xy
        float fTxz = fTz * x; // 2xz
        float fTyy = fTy * y; // 2y^2
        float fTyz = fTz * y; // 2yz
        float fTzz = fTz * z; // 2z^2

        kRot[0][0] = 1.0f - (fTyy + fTzz); // 1 - 2y^2 - 2z^2
        kRot[0][1] = fTxy - fTwz;          // 2xy - 2wz
        kRot[0][2] = fTxz + fTwy;          // 2xz + 2wy
        kRot[0][3] = 0;
        kRot[1][0] = fTxy + fTwz;          // 2xy + 2wz
        kRot[1][1] = 1.0f - (fTxx + fTzz); // 1 - 2x^2 - 2z^2
        kRot[1][2] = fTyz - fTwx;          // 2yz - 2wx
        kRot[1][3] = 0;
        kRot[2][0] = fTxz - fTwy;          // 2xz - 2wy
        kRot[2][1] = fTyz + fTwx;          // 2yz + 2wx
        kRot[2][2] = 1.0f - (fTxx + fTyy); // 1 - 2x^2 - 2y^2
        kRot[2][3] = 0;
        kRot[3][0] = 0;
        kRot[3][1] = 0;
        kRot[3][2] = 0;
        kRot[3][3] = 1;
    }

    void Quaternion::fromAngleAxis(Radian const& angle, Vec3 const& axis)
    {
        // ASSERT:  axis[] is unit length
        //
        // The quaternion representing the rotation is
        //   q = cos(A/2)+sin(A/2)*(x*i+y*j+z*k)
        Radian half_angle(angle * 0.5);
        float  sin_v = sin(half_angle);
        w = cos(half_angle);
        x = sin_v * axis.x;
        y = sin_v * axis.y;
        z = sin_v * axis.z;
    }

    Quaternion Quaternion::getQuaternionFromAngleAxis(Radian const& angle, Vec3 const& axis)
    {
        Quaternion q;
        q.fromAngleAxis(angle, axis);
        return q;
    }

    void Quaternion::fromDirection(Vec3 const& direction, Vec3 const& up_direction)
    {
        Vec3 forward_direction = direction;
        forward_direction.z = 0.0f;
        forward_direction.normalise();

        Vec3 left_direction = up_direction.crossProduct(forward_direction);

        fromAxes(left_direction, -forward_direction, up_direction);
        normalise();
    }

    Quaternion Quaternion::getQuaternionFromDirection(Vec3 const& direction, Vec3 const& up_direction)
    {
        Quaternion object_orientation;
        object_orientation.fromDirection(direction, up_direction);
        return object_orientation;
    }

    void Quaternion::toAngleAxis(Radian& angle, Vec3& axis) const
    {
        // The quaternion representing the rotation is
        //   q = cos(A/2)+sin(A/2)*(x*i+y*j+z*k)

        float sqr_len = x * x + y * y + z * z;
        if (sqr_len > 0.0)
        {
            angle = acos(w) * 2.0;
            float inv_len = invSqrt(sqr_len);
            axis.x = x * inv_len;
            axis.y = y * inv_len;
            axis.z = z * inv_len;
        }
        else
        {
            // angle is 0 (mod 2*pi), so any axis will do
            angle = Radian(0.0);
            axis.x = 1.0;
            axis.y = 0.0;
            axis.z = 0.0;
        }
    }

    //-----------------------------------------------------------------------
    void Quaternion::fromAxes(Vec3 const& xaxis, Vec3 const& yaxis, Vec3 const& zaxis)
    {
        Mat3 rot;

        rot[0][0] = xaxis.x;
        rot[1][0] = xaxis.y;
        rot[2][0] = xaxis.z;

        rot[0][1] = yaxis.x;
        rot[1][1] = yaxis.y;
        rot[2][1] = yaxis.z;

        rot[0][2] = zaxis.x;
        rot[1][2] = zaxis.y;
        rot[2][2] = zaxis.z;

        fromRotationMatrix(rot);
    }
    //-----------------------------------------------------------------------
    Vec3 Quaternion::xAxis() const
    {
        // float tx  = 2.0*x;
        float ty = 2.0f * y;
        float tz = 2.0f * z;
        float twy = ty * w;
        float twz = tz * w;
        float txy = ty * x;
        float txz = tz * x;
        float tyy = ty * y;
        float tzz = tz * z;

        return Vec3(1.0f - (tyy + tzz), txy + twz, txz - twy);
    }
    //-----------------------------------------------------------------------
    Vec3 Quaternion::yAxis() const
    {
        float tx = 2.0f * x;
        float ty = 2.0f * y;
        float tz = 2.0f * z;
        float twx = tx * w;
        float twz = tz * w;
        float txx = tx * x;
        float txy = ty * x;
        float tyz = tz * y;
        float tzz = tz * z;

        return Vec3(txy - twz, 1.0f - (txx + tzz), tyz + twx);
    }
    //-----------------------------------------------------------------------
    Vec3 Quaternion::zAxis() const
    {
        float tx = 2.0f * x;
        float ty = 2.0f * y;
        float tz = 2.0f * z;
        float twx = tx * w;
        float twy = ty * w;
        float txx = tx * x;
        float txz = tz * x;
        float tyy = ty * y;
        float tyz = tz * y;

        return Vec3(txz + twy, tyz - twx, 1.0f - (txx + tyy));
    }
    //-----------------------------------------------------------------------
    void Quaternion::toAxes(Vec3& xaxis, Vec3& yaxis, Vec3& zaxis) const
    {
        Mat3 rot;

        toRotationMatrix(rot);

        xaxis.x = rot[0][0];
        xaxis.y = rot[1][0];
        xaxis.z = rot[2][0];

        yaxis.x = rot[0][1];
        yaxis.y = rot[1][1];
        yaxis.z = rot[2][1];

        zaxis.x = rot[0][2];
        zaxis.y = rot[1][2];
        zaxis.z = rot[2][2];
    }

    Vec3 Quaternion::operator*(Vec3 const& v) const
    {
        // nVidia SDK implementation
        Vec3 uv, uuv;
        Vec3 qvec(x, y, z);
        uv = qvec.crossProduct(v);
        uuv = qvec.crossProduct(uv);
        uv *= (2.0f * w);
        uuv *= 2.0f;

        return v + uv + uuv;
    }

    Radian Quaternion::getYaw(bool reproject_axis) const
    {
        if (reproject_axis)
        {
            // roll = atan2(localx.y, localx.x)
            // pick parts of xAxis() implementation that we need
            //  float tx  = 2.0*x;
            float ty = 2.0f * y;
            float tz = 2.0f * z;
            float twz = tz * w;
            float txy = ty * x;
            float tyy = ty * y;
            float tzz = tz * z;

            return Radian(atan2(txy + twz, 1.0f - (tyy + tzz)));
        }
        else
        {
            return Radian(atan2(2 * (x * y + w * z), w * w + x * x - y * y - z * z));
        }
    }
    //-----------------------------------------------------------------------
    Radian Quaternion::getPitch(bool reproject_axis) const
    {
        if (reproject_axis)
        {
            // pitch = atan2(localy.z, localy.y)
            // pick parts of yAxis() implementation that we need
            float tx = 2.0f * x;
            //  float ty  = 2.0f*y;
            float tz = 2.0f * z;
            float twx = tx * w;
            float txx = tx * x;
            float tyz = tz * y;
            float tzz = tz * z;

            return Radian(atan2(tyz + twx, 1.0f - (txx + tzz)));
        }
        else
        {
            // internal version
            return Radian(atan2(2 * (y * z + w * x), w * w - x * x - y * y + z * z));
        }
    }
    //-----------------------------------------------------------------------
    Radian Quaternion::getRoll(bool reproject_axis) const
    {
        if (reproject_axis)
        {
            // yaw = atan2(localz.x, localz.z)
            // pick parts of zAxis() implementation that we need
            float tx = 2.0f * x;
            float ty = 2.0f * y;
            float tz = 2.0f * z;
            float twy = ty * w;
            float txx = tx * x;
            float txz = tz * x;
            float tyy = ty * y;

            return Radian(atan2(txz + twy, 1.0f - (txx + tyy)));
        }
        else
        {
            // internal version
            return Radian(asin(-2 * (x * z - w * y)));
        }
    }

    Quaternion Quaternion::sLerp(float t, Quaternion const& kp, Quaternion const& kq, bool shortest_path)
    {
        float      cos_v = kp.dot(kq);
        Quaternion kt;

        // Do we need to invert rotation?
        if (cos_v < 0.0f && shortest_path)
        {
            cos_v = -cos_v;
            kt = -kq;
        }
        else
        {
            kt = kq;
        }

        if (abs(cos_v) < 1 - k_epsilon)
        {
            // Standard case (slerp)
            float  sin_v = sqrt(1 - sqr(cos_v));
            Radian angle = atan2(sin_v, cos_v);
            float  inv_sin = 1.0f / sin_v;
            float  coeff0 = sin(angle * (1.0f - t)) * inv_sin;
            float  coeff1 = sin(angle * t) * inv_sin;
            return coeff0 * kp + coeff1 * kt;
        }
        else
        {
            // There are two situations:
            // 1. "rkP" and "rkQ" are very close (fCos ~= +1), so we can do a linear
            //    interpolation safely.
            // 2. "rkP" and "rkQ" are almost inverse of each other (fCos ~= -1), there
            //    are an infinite number of possibilities interpolation. but we haven't
            //    have method to fix this case, so just use linear interpolation here.
            Quaternion r = (1.0f - t) * kp + t * kt;
            // taking the complement requires renormalization
            r.normalise();
            return r;
        }
    }

    Quaternion Quaternion::nLerp(float t, Quaternion const& kp, Quaternion const& kq, bool shortest_path)
    {
        Quaternion result;
        float      cos_value = kp.dot(kq);
        if (cos_value < 0.0f && shortest_path)
        {
            result = kp + t * ((-kq) - kp);
        }
        else
        {
            result = kp + t * (kq - kp);
        }
        result.normalise();
        return result;
    }
} // namespace Math
} // namespace MarsEngine
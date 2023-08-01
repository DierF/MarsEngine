#pragma once

#include "Runtime/Core/Math/Vector3.h"
#include "Runtime/Core/Math/Vector4.h"

namespace MarsEngine
{
    class RenderScene;
    class RenderCamera;

    static inline uint32_t roundUp(uint32_t value, uint32_t alignment)
    {
        uint32_t temp = value + alignment - static_cast<uint32_t>(1);
        return (temp - temp % alignment);
    }

    // TODO: support cluster lighting
    struct ClusterFrustum
    {
        // we don't consider the near and far plane currently
        Vec4 m_plane_right;
        Vec4 m_plane_left;
        Vec4 m_plane_top;
        Vec4 m_plane_bottom;
        Vec4 m_plane_near;
        Vec4 m_plane_far;
    };

    struct BoundingBox
    {
        Vec3 min_bound {std::numeric_limits<float>::max(),
                              std::numeric_limits<float>::max(),
                              std::numeric_limits<float>::max()};
        Vec3 max_bound {std::numeric_limits<float>::min(),
                              std::numeric_limits<float>::min(),
                              std::numeric_limits<float>::min()};

        BoundingBox() {}

        BoundingBox(Vec3 const& minv, Vec3 const& maxv)
        {
            min_bound = minv;
            max_bound = maxv;
        }

        void merge(BoundingBox const& rhs)
        {
            min_bound.makeFloor(rhs.min_bound);
            max_bound.makeCeil(rhs.max_bound);
        }

        void merge(Vec3 const& point)
        {
            min_bound.makeFloor(point);
            max_bound.makeCeil(point);
        }
    };

    struct BoundingSphere
    {
        Vec3   m_center;
        float        m_radius;
    };

    struct FrustumPoints
    {
        Vec3 m_frustum_points;
    };

    ClusterFrustum CreateClusterFrustumFromMatrix(Mat4 mat,
                                                  float      x_left,
                                                  float      x_right,
                                                  float      y_top,
                                                  float      y_bottom,
                                                  float      z_near,
                                                  float      z_far);

    bool TiledFrustumIntersectBox(ClusterFrustum const& f, BoundingBox const& b);

    BoundingBox BoundingBoxTransform(BoundingBox const& b, Mat4 const& m);

    bool BoxIntersectsWithSphere(BoundingBox const& b, BoundingSphere const& s);

    Mat4 CalculateDirectionalLightCamera(RenderScene& scene, RenderCamera& camera);
} // namespace MarsEngine

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
        Math::Vec4 m_plane_right;
        Math::Vec4 m_plane_left;
        Math::Vec4 m_plane_top;
        Math::Vec4 m_plane_bottom;
        Math::Vec4 m_plane_near;
        Math::Vec4 m_plane_far;
    };

    struct BoundingBox
    {
        Math::Vec3 min_bound {std::numeric_limits<float>::max(),
                              std::numeric_limits<float>::max(),
                              std::numeric_limits<float>::max()};
        Math::Vec3 max_bound {std::numeric_limits<float>::min(),
                              std::numeric_limits<float>::min(),
                              std::numeric_limits<float>::min()};

        BoundingBox() {}

        BoundingBox(Math::Vec3 const& minv, Math::Vec3 const& maxv)
        {
            min_bound = minv;
            max_bound = maxv;
        }

        void merge(BoundingBox const& rhs)
        {
            min_bound.makeFloor(rhs.min_bound);
            max_bound.makeCeil(rhs.max_bound);
        }

        void merge(Math::Vec3 const& point)
        {
            min_bound.makeFloor(point);
            max_bound.makeCeil(point);
        }
    };

    struct BoundingSphere
    {
        Math::Vec3   m_center;
        float        m_radius;
    };

    struct FrustumPoints
    {
        Math::Vec3 m_frustum_points;
    };

    ClusterFrustum CreateClusterFrustumFromMatrix(Math::Mat4 mat,
                                                  float      x_left,
                                                  float      x_right,
                                                  float      y_top,
                                                  float      y_bottom,
                                                  float      z_near,
                                                  float      z_far);

    bool TiledFrustumIntersectBox(ClusterFrustum const& f, BoundingBox const& b);

    BoundingBox BoundingBoxTransform(BoundingBox const& b, Math::Mat4 const& m);

    bool BoxIntersectsWithSphere(BoundingBox const& b, BoundingSphere const& s);

    Math::Mat4 CalculateDirectionalLightCamera(RenderScene& scene, RenderCamera& camera);
} // namespace MarsEngine

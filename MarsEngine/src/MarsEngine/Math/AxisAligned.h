#pragma once

#include "Vector3.h"

#include <limits>

namespace MarsEngine
{
namespace Math
{
    //REFLECTION_TYPE(AxisAlignedBox)
        //CLASS(AxisAlignedBox, Fields)
    class AxisAlignedBox
    {
        //REFLECTION_BODY(AxisAlignedBox)

    public:
        AxisAlignedBox() {}
        AxisAlignedBox(Vec3 const& center, Vec3 const& half_extent);

        void merge(Vec3 const& new_point);
        void update(Vec3 const& center, Vec3 const& half_extent);

        Vec3 const& getCenter() const { return m_center; }
        Vec3 const& getHalfExtent() const { return m_half_extent; }
        Vec3 const& getMinCorner() const { return m_min_corner; }
        Vec3 const& getMaxCorner() const { return m_max_corner; }

    private:
        Vec3 m_center{ Vec3::ZERO };
        Vec3 m_half_extent{ Vec3::ZERO };

        Vec3 m_min_corner{
            std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max() };
        Vec3 m_max_corner{
            -std::numeric_limits<float>::max(), -std::numeric_limits<float>::max(), -std::numeric_limits<float>::max() };
    };
} // namespace Math
} // namespace MarsEngine
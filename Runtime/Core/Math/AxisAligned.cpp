#include "Runtime/Core/Math/AxisAligned.h"

namespace MarsEngine
{
namespace Math
{
    AxisAlignedBox::AxisAlignedBox(Vec3 const& center, Vec3 const& half_extent) { update(center, half_extent); }

    void AxisAlignedBox::merge(Vec3 const& new_point)
    {
        m_min_corner.makeFloor(new_point);
        m_max_corner.makeCeil(new_point);

        m_center = 0.5f * (m_min_corner + m_max_corner);
        m_half_extent = m_center - m_min_corner;
    }

    void AxisAlignedBox::update(Vec3 const& center, Vec3 const& half_extent)
    {
        m_center = center;
        m_half_extent = half_extent;
        m_min_corner = center - half_extent;
        m_max_corner = center + half_extent;
    }
} // namespace Math
} // namespace MarsEngine
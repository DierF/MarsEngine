#include "Runtime/Resource/ResType/Components/RigidBody.h"
#include "Runtime/Core/Base/Macro.h"

namespace MarsEngine
{
    RigidBodyShape::RigidBodyShape(RigidBodyShape const& res) :
        m_local_transform(res.m_local_transform)
    {
        if (res.m_geometry.getTypeName() == "Box")
        {
            m_type     = RigidBodyShapeType::box;
            m_geometry = MARS_REFLECTION_NEW(Box);
            MARS_REFLECTION_DEEP_COPY(Box, m_geometry, res.m_geometry);
        }
        else
        {
            LOG_ERROR("Not supported shape type!");
        }
    }

    RigidBodyShape::~RigidBodyShape()
    {
        MARS_REFLECTION_DELETE(m_geometry);
    }
} // namespace MarsEngine
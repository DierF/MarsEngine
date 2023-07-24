#pragma once

#include "Runtime/Resource/ResType/Data/BasicShape.h"
#include "Runtime/Core/Math/AxisAligned.h"
#include "Runtime/Core/Math/Transform.h"
#include "Runtime/Core/Meta/Reflection/Reflection.h"

namespace MarsEngine
{
    enum class RigidBodyShapeType : unsigned char
    {
        box,
        sphere,
        capsule,
        invalid
    };

    REFLECTION_TYPE(RigidBodyShape)
    CLASS(RigidBodyShape, WhiteListFields)
    {
        REFLECTION_BODY(RigidBodyShape);

    public:
        Math::Transform      m_global_transform;
        Math::AxisAlignedBox m_bounding_box;
        RigidBodyShapeType   m_type {RigidBodyShapeType::invalid};

        META(Enable)
        Math::Transform m_local_transform;
        META(Enable)
        Reflection::ReflectionPtr<Geometry> m_geometry;

        RigidBodyShape() = default;
        RigidBodyShape(const RigidBodyShape& res);

        ~RigidBodyShape();
    };

    REFLECTION_TYPE(RigidBodyComponentRes)
    CLASS(RigidBodyComponentRes, Fields)
    {
        REFLECTION_BODY(RigidBodyComponentRes);

    public:
        std::vector<RigidBodyShape> m_shapes;
        float                       m_inverse_mass;
        int                         m_actor_type;
    };
} // namespace MarsEngine
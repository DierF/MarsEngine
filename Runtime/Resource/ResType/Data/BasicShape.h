#pragma once

#include "Runtime/Core/Math/Vector3.h"
#include "Runtime/Core/Meta/Reflection/Reflection.h"

namespace MarsEngine
{
    REFLECTION_TYPE(Geometry)
    CLASS(Geometry, Fields)
    {
        REFLECTION_BODY(Geometry);

    public:
        virtual ~Geometry() {}
    };

    REFLECTION_TYPE(Box)
    CLASS(Box : public Geometry, Fields)
    {
        REFLECTION_BODY(Box);

    public:
        ~Box() override {}

        Math::Vec3 m_half_extents {0.5f, 0.5f, 0.5f};
    };

    REFLECTION_TYPE(Sphere)
    CLASS(Sphere : public Geometry, Fields)
    {
        REFLECTION_BODY(Sphere);

    public:
        ~Sphere() override {}
        float m_radius {0.5f};
    };

    REFLECTION_TYPE(Capsule)
    CLASS(Capsule : public Geometry, Fields)
    {
        REFLECTION_BODY(Capsule);

    public:
        ~Capsule() override {}
        float m_radius {0.3f};
        float m_half_height {0.7f};
    };
} // namespace MarsEngine
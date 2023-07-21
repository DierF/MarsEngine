#pragma once

#include "Runtime/Core/Meta/Reflection/Reflection.h"

#include "Runtime/Core/Math/Vector3.h"

namespace MarsEngine
{
    REFLECTION_TYPE(Color)
    CLASS(Color, Fields)
    {
        REFLECTION_BODY(Color);

        Math::Vec3 toVec3() const { return Math::Vec3(r, g, b); }

    public:
        float r;
        float g;
        float b;
    };
} // namespace MarsEngine
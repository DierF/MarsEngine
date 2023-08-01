#pragma once

#include "Runtime/Core/Meta/Reflection/Reflection.h"

#include <string>
#include <vector>

namespace MarsEngine
{
    REFLECTION_TYPE(AnimSkelMap)
    CLASS(AnimSkelMap, Fields)
    {
        REFLECTION_BODY(AnimSkelMap);

    public:
        std::vector<int> convert;
    };

} // namespace MarsEngine
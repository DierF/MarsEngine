#pragma once

#include "Runtime/Core/Meta/Reflection/Reflection.h"

#include <string>
#include <vector>

namespace MarsEngine
{
    REFLECTION_TYPE(BoneBlendMask)
    CLASS(BoneBlendMask, Fields)
    {
        REFLECTION_BODY(BoneBlendMask);

    public:
        std::string      skeleton_file_path;
        std::vector<int> enabled;
    };

} // namespace MarsEngine
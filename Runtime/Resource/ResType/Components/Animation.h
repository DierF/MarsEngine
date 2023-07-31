#pragma once

#include "Runtime/Core/Meta/Reflection/Reflection.h"
#include "Runtime/Resource/ResType/Data/BlendState.h"

#include <string>
#include <vector>

namespace MarsEngine
{

    REFLECTION_TYPE(AnimationResultElement)
    CLASS(AnimationResultElement, WhiteListFields)
    {
        REFLECTION_BODY(AnimationResultElement);

    public:
        int        index;
        Math::Mat4_ transform;
    };

    REFLECTION_TYPE(AnimationResult)
    CLASS(AnimationResult, Fields)
    {
        REFLECTION_BODY(AnimationResult);

    public:
        std::vector<AnimationResultElement> node;
    };

    REFLECTION_TYPE(AnimationComponentRes)
    CLASS(AnimationComponentRes, Fields)
    {
        REFLECTION_BODY(AnimationComponentRes);

    public:
        std::string skeleton_file_path;
        BlendState  blend_state;
        // animation to skeleton map
        float       frame_position; // 0-1

        META(Disable)
        AnimationResult animation_result;
    };

} // namespace MarsEngine
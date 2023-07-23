#pragma once

#include "Runtime/Core/Math/Transform.h"
#include "Runtime/Core/Meta/Reflection/Reflection.h"

#include <string>
#include <vector>

namespace MarsEngine
{
    REFLECTION_TYPE(AnimNodeMap)
    CLASS(AnimNodeMap, Fields)
    {
        REFLECTION_BODY(AnimNodeMap);

    public:
        std::vector<std::string> convert;
    };

    REFLECTION_TYPE(AnimationChannel)
    CLASS(AnimationChannel, Fields)
    {
        REFLECTION_BODY(AnimationChannel);

    public:
        std::string             name;
        std::vector<Math::Vec3>    position_keys;
        std::vector<Math::Quaternion> rotation_keys;
        std::vector<Math::Vec3>    scaling_keys;
    };

    REFLECTION_TYPE(AnimationClip)
    CLASS(AnimationClip, Fields)
    {
        REFLECTION_BODY(AnimationClip);

    public:
        int                           total_frame {0};
        int                           node_count {0};
        std::vector<AnimationChannel> node_channels;
    };

    REFLECTION_TYPE(AnimationAsset)
    CLASS(AnimationAsset, Fields)
    {
        REFLECTION_BODY(AnimationAsset);

    public:
        AnimNodeMap   node_map;
        AnimationClip clip_data;
        std::string   skeleton_file_path;
    };

} // namespace MarsEngine
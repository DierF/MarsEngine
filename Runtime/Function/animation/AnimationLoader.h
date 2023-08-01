#pragma once

#include "Runtime/Resource/ResType/Data/AnimationClip.h"
#include "Runtime/Resource/ResType/Data/AnimationSkeletonNodeMap.h"
#include "Runtime/Resource/ResType/Data/SkeletonData.h"
#include "Runtime/Resource/ResType/Data/SkeletonMask.h"

#include <memory>
#include <string>

namespace MarsEngine
{
    class AnimationLoader
    {
    public:
        std::shared_ptr<AnimationClip> loadAnimationClipData(std::string animation_clip_url);
        std::shared_ptr<SkeletonData>  loadSkeletonData(std::string skeleton_data_url);
        std::shared_ptr<AnimSkelMap>   loadAnimSkelMap(std::string anim_skel_map_url);
        std::shared_ptr<BoneBlendMask> loadSkeletonMask(std::string skeleton_mask_file_url);
    };
} // namespace MarsEngine
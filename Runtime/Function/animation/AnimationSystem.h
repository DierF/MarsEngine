#pragma once

#include "Runtime/Resource/ResType/Data/AnimationClip.h"
#include "Runtime/Resource/ResType/Data/AnimationSkeletonNodeMap.h"
#include "Runtime/Resource/ResType/Data/BlendState.h"
#include "Runtime/Resource/ResType/Data/SkeletonData.h"
#include "Runtime/Resource/ResType/Data/SkeletonMask.h"

#include <map>
#include <memory>
#include <string>

namespace MarsEngine
{
    class AnimationManager
    {
    private:
        static std::map<std::string, std::shared_ptr<SkeletonData>>  m_skeleton_definition_cache;
        static std::map<std::string, std::shared_ptr<AnimationClip>> m_animation_data_cache;
        static std::map<std::string, std::shared_ptr<AnimSkelMap>>   m_animation_skeleton_map_cache;
        static std::map<std::string, std::shared_ptr<BoneBlendMask>> m_skeleton_mask_cache;

    public:
        static std::shared_ptr<SkeletonData>  tryLoadSkeleton(std::string file_path);
        static std::shared_ptr<AnimationClip> tryLoadAnimation(std::string file_path);
        static std::shared_ptr<AnimSkelMap>   tryLoadAnimationSkeletonMap(std::string file_path);
        static std::shared_ptr<BoneBlendMask> tryLoadSkeletonMask(std::string file_path);
        static BlendStateWithClipData         getBlendStateWithClipData(BlendState const& blend_state);

        AnimationManager() = default;
    };

} // namespace MarsEngine
#pragma once

#include "Runtime/Function/Animation/Node.h"
#include "Runtime/Resource/ResType/Components/Animation.h"

namespace MarsEngine
{
    class SkeletonData;
    class BlendStateWithClipData;

    class Skeleton
    {
    private:
        bool  m_is_flat {false};
        int   m_bone_count {0};
        Bone* m_bones {nullptr};

    public:
        ~Skeleton();

        void            buildSkeleton(SkeletonData const& skeleton_definition);
        void            applyAnimation(BlendStateWithClipData const& blend_state);
        AnimationResult outputAnimationResult();
        void            resetSkeleton();
        Bone const*     getBones() const;
        int32_t         getBonesCount() const;
    };
} // namespace MarsEngine

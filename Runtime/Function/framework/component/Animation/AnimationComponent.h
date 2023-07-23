#pragma once

#include "Runtime/Function/Animation/Skeleton.h"
#include "Runtime/Function/Framework/Component/Component.h"
#include "Runtime/Resource/ResType/Components/Animation.h"

namespace MarsEngine
{
    REFLECTION_TYPE(AnimationComponent)
    CLASS(AnimationComponent : public Component, WhiteListFields)
    {
        REFLECTION_BODY(AnimationComponent)

    public:
        AnimationComponent() = default;

        void postLoadResource(std::weak_ptr<GObject> parent_object) override;

        void tick(float delta_time) override;

        AnimationResult const& getResult() const;

        Skeleton const& getSkeleton() const;

    protected:
        META(Enable)
        AnimationComponentRes m_animation_res;

        Skeleton m_skeleton;
    };
} // namespace MarsEngine

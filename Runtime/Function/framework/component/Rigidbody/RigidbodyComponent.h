#pragma once

#include "Runtime/Function/Framework/Component/Component.h"
#include "Runtime/Resource/ResType/Components/RigidBody.h"

namespace MarsEngine
{
    REFLECTION_TYPE(RigidBodyComponent)
    CLASS(RigidBodyComponent : public Component, WhiteListFields)
    {
        REFLECTION_BODY(RigidBodyComponent)
    public:
        RigidBodyComponent() = default;
        ~RigidBodyComponent() override;

        void postLoadResource(std::weak_ptr<GObject> parent_object) override;

        void tick(float delta_time) override {}
        void updateGlobalTransform(Math::Transform const& transform, bool is_scale_dirty);
        void getShapeBoundingBoxes(std::vector<Math::AxisAlignedBox>& out_boudning_boxes) const;

    protected:
        void createRigidBody(Math::Transform const& global_transform);
        void removeRigidBody();

        META(Enable)
        RigidBodyComponentRes m_rigidbody_res;

        uint32_t m_rigidbody_id {0xffffffff};
    };
} // namespace MarsEngine

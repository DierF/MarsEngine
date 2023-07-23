#pragma once

#include "Runtime/Function/Framework/Component/Component.h"
#include "Runtime/Function/Framework/Object/Object.h"
#include "Runtime/Core/Math/Transform.h"
#include "Runtime/Core/Math/Matrix4.h"

namespace MarsEngine
{
    REFLECTION_TYPE(TransformComponent)
    CLASS(TransformComponent : public Component, WhiteListFields)
    {
        REFLECTION_BODY(TransformComponent)

    public:
        TransformComponent() = default;

        void postLoadResource(std::weak_ptr<GObject> parent_object) override;

        Math::Vec3    getPosition() const { return m_transform_buffer[m_current_index].m_position; }
        Math::Vec3    getScale() const { return m_transform_buffer[m_current_index].m_scale; }
        Math::Quaternion getRotation() const { return m_transform_buffer[m_current_index].m_rotation; }

        void setPosition(Math::Vec3 const& new_translation);

        void setScale(Math::Vec3 const& new_scale);

        void setRotation(Math::Quaternion const& new_rotation);

        Math::Transform const& getTransformConst() const { return m_transform_buffer[m_current_index]; }
        Math::Transform&       getTransform() { return m_transform_buffer[m_next_index]; }

        Math::Mat4 getMatrix() const { return m_transform_buffer[m_current_index].getMatrix(); }

        void tick(float delta_time) override;

        void tryUpdateRigidBodyComponent();

    protected:
        META(Enable)
        Math::Transform m_transform;

        Math::Transform m_transform_buffer[2];
        size_t    m_current_index {0};
        size_t    m_next_index {1};
    };
} // namespace MarsEngine

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

        Vec3    getPosition() const { return m_transform_buffer[m_current_index].m_position; }
        Vec3    getScale() const { return m_transform_buffer[m_current_index].m_scale; }
        Quaternion getRotation() const { return m_transform_buffer[m_current_index].m_rotation; }

        void setPosition(Vec3 const& new_translation);

        void setScale(Vec3 const& new_scale);

        void setRotation(Quaternion const& new_rotation);

        Transform const& getTransformConst() const { return m_transform_buffer[m_current_index]; }
        Transform&       getTransform() { return m_transform_buffer[m_next_index]; }

        Mat4 getMatrix() const { return m_transform_buffer[m_current_index].getMatrix(); }

        void tick(float delta_time) override;

        void tryUpdateRigidBodyComponent();

    protected:
        META(Enable)
        Transform m_transform;

        Transform m_transform_buffer[2];
        size_t    m_current_index {0};
        size_t    m_next_index {1};
    };
} // namespace MarsEngine

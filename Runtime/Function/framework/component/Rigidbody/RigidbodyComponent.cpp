#include "Runtime/Function/Framework/Component/Rigidbody/RigidbodyComponent.h"
#include "Runtime/Function/Framework/Component/Transform/TransformComponent.h"
#include "Runtime/Function/Framework/Object/Object.h"
#include "Runtime/Function/Framework/World/WorldManager.h"
#include "Runtime/Function/Global/GlobalContext.h"
#include "Runtime/Function/Physics/PhysicsScene.h"
#include "Runtime/Core/Base/Macro.h"
#include "Runtime/Engine.h"

namespace MarsEngine
{
    void RigidBodyComponent::postLoadResource(std::weak_ptr<GObject> parent_object)
    {
        m_parent_object = parent_object;

        TransformComponent const* parent_transform = m_parent_object.lock()->tryGetComponentConst(TransformComponent);
        if (parent_transform == nullptr)
        {
            LOG_ERROR("No transform component in the object");
            return;
        }

        std::shared_ptr<PhysicsScene> physics_scene =
            g_runtime_global_context.m_world_manager->getCurrentActivePhysicsScene().lock();
        ASSERT(physics_scene);

        m_rigidbody_id = physics_scene->createRigidBody(parent_transform->getTransformConst(), m_rigidbody_res);
    }

    RigidBodyComponent::~RigidBodyComponent()
    {
        std::shared_ptr<PhysicsScene> physics_scene =
            g_runtime_global_context.m_world_manager->getCurrentActivePhysicsScene().lock();
        ASSERT(physics_scene);

        physics_scene->removeRigidBody(m_rigidbody_id);
    }

    void RigidBodyComponent::createRigidBody(Transform const& global_transform)
    {
        std::shared_ptr<PhysicsScene> physics_scene =
            g_runtime_global_context.m_world_manager->getCurrentActivePhysicsScene().lock();
        ASSERT(physics_scene);

        m_rigidbody_id = physics_scene->createRigidBody(global_transform, m_rigidbody_res);
    }

    void RigidBodyComponent::removeRigidBody()
    {
        std::shared_ptr<PhysicsScene> physics_scene =
            g_runtime_global_context.m_world_manager->getCurrentActivePhysicsScene().lock();
        ASSERT(physics_scene);

        physics_scene->removeRigidBody(m_rigidbody_id);
    }

    void RigidBodyComponent::updateGlobalTransform(Transform const& transform, bool is_scale_dirty)
    {
        if (is_scale_dirty)
        {
            removeRigidBody();

            createRigidBody(transform);
        }
        else
        {
            std::shared_ptr<PhysicsScene> physics_scene =
                g_runtime_global_context.m_world_manager->getCurrentActivePhysicsScene().lock();
            ASSERT(physics_scene);

            physics_scene->updateRigidBodyGlobalTransform(m_rigidbody_id, transform);
        }
    }

    void RigidBodyComponent::getShapeBoundingBoxes(std::vector<AxisAlignedBox>& out_bounding_boxes) const
    {
        std::shared_ptr<PhysicsScene> physics_scene =
            g_runtime_global_context.m_world_manager->getCurrentActivePhysicsScene().lock();

        ASSERT(physics_scene);

        physics_scene->getShapeBoundingBoxes(m_rigidbody_id, out_bounding_boxes);
    }

} // namespace MarsEngine

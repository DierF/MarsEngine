#include "Runtime/Function/Controller/CharacterController.h"
#include "Runtime/Function/Framework/Component/Motor/MotorComponent.h"
#include "Runtime/Function/Framework/World/WorldManager.h"
#include "Runtime/Function/Global/GlobalContext.h"
#include "Runtime/Function/Physics/PhysicsScene.h"
#include "Runtime/Core/Base/Macro.h"
#include "Runtime/Core/Math/Degree.h"
#include "Runtime/Core/Math/Radian.h"

namespace MarsEngine
{
    CharacterController::CharacterController(Capsule const& capsule) : m_capsule(capsule)
    {
        m_rigidbody_shape                                    = RigidBodyShape();
        m_rigidbody_shape.m_geometry                         = MARS_REFLECTION_NEW(Capsule);
        *static_cast<Capsule*>(m_rigidbody_shape.m_geometry) = m_capsule;

        m_rigidbody_shape.m_type = RigidBodyShapeType::capsule;

        Math::Quaternion orientation;
        orientation.fromAngleAxis(Math::Radian(Math::Degree(90.f)), Math::Vec3::UNIT_X);

        m_rigidbody_shape.m_local_transform =
            Math::Transform(Math::Vec3(0, 0, capsule.m_half_height + capsule.m_radius), orientation, Math::Vec3::UNIT_SCALE);
    }

    Math::Vec3 CharacterController::move(Math::Vec3 const& current_position, Math::Vec3 const& displacement)
    {
        std::shared_ptr<PhysicsScene> physics_scene =
            g_runtime_global_context.m_world_manager->getCurrentActivePhysicsScene().lock();
        ASSERT(physics_scene);

        Math::Vec3 final_position = current_position + displacement;

        Math::Transform final_transform = Math::Transform(final_position, Math::Quaternion::IDENTITY, Math::Vec3::UNIT_SCALE);

        if (physics_scene->isOverlap(m_rigidbody_shape, final_transform.getMatrix()))
        {
            final_position = current_position;
        }

        return final_position;
    }
} // namespace MarsEngine

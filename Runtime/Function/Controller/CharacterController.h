#pragma once

#include "Runtime/Core/Math/Vector3.h"
#include "Runtime/Resource/ResType/Components/RigidBody.h"
#include "Runtime/Resource/ResType/Data/BasicShape.h"

namespace MarsEngine
{
    enum SweepPass
    {
        SWEEP_PASS_UP,
        SWEEP_PASS_SIDE,
        SWEEP_PASS_DOWN,
        SWEEP_PASS_SENSOR
    };

    class Controller
    {
    public:
        virtual ~Controller() = default;

        virtual Math::Vec3 move(Math::Vec3 const& current_position, Math::Vec3 const& displacement) = 0;
    };

    class CharacterController : public Controller
    {
    public:
        CharacterController(Capsule const& capsule);
        ~CharacterController() = default;

        Math::Vec3 move(Math::Vec3 const& current_position, Math::Vec3 const& displacement) override;

    private:
        Capsule        m_capsule;
        RigidBodyShape m_rigidbody_shape;
    };
} // namespace MarsEngine

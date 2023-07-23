#include "Runtime/Function/Framework/Component/Motor/MotorComponent.h"
#include "Runtime/Function/Framework/Component/Animation/AnimationComponent.h"
#include "Runtime/Function/Framework/Component/Camera/CameraComponent.h"
#include "Runtime/Function/Framework/Component/Transform/TransformComponent.h"
#include "Runtime/Function/Framework/Level/Level.h"
#include "Runtime/Function/Framework/Object/Object.h"
#include "Runtime/Function/Framework/World/WorldManager.h"
#include "Runtime/Function/Character/Character.h"
#include "Runtime/Function/Controller/CharacterController.h"
#include "Runtime/Function/Global/GlobalContext.h"
#include "Runtime/Function/Input/InputSystem.h"
#include "Runtime/Function/Physics/PhysicsScene.h"
#include "Runtime/Core/Base/Macro.h"
#include "Runtime/Core/Math/MathHeaders.h"

namespace MarsEngine
{
    void MotorComponent::postLoadResource(std::weak_ptr<GObject> parent_object)
    {
        m_parent_object = parent_object;

        if (m_motor_res.m_controller_config.getTypeName() == "PhysicsControllerConfig")
        {
            m_controller_type = ControllerType::physics;
            PhysicsControllerConfig* controller_config =
                static_cast<PhysicsControllerConfig*>(m_motor_res.m_controller_config);
            m_controller = new CharacterController(controller_config->m_capsule_shape);
        }
        else if (m_motor_res.m_controller_config != nullptr)
        {
            m_controller_type = ControllerType::invalid;
            LOG_ERROR("invalid controller type, not able to move");
        }

        TransformComponent const* transform_component = parent_object.lock()->tryGetComponentConst(TransformComponent);

        m_target_position = transform_component->getPosition();
    }

    void MotorComponent::getOffStuckDead() { LOG_INFO("Some get off stuck dead logic"); }
    MotorComponent::~MotorComponent()
    {
        if (m_controller_type == ControllerType::physics)
        {
            delete m_controller;
            m_controller = nullptr;
        }
    }

    void MotorComponent::tick(float delta_time) { tickPlayerMotor(delta_time); }

    void MotorComponent::tickPlayerMotor(float delta_time)
    {
        if (!m_parent_object.lock())
            return;

        std::shared_ptr<Level> current_level = g_runtime_global_context.m_world_manager->getCurrentActiveLevel().lock();
        std::shared_ptr<Character> current_character = current_level->getCurrentActiveCharacter().lock();
        if (current_character == nullptr)
            return;

        if (current_character->getObjectID() != m_parent_object.lock()->getID())
            return;

        TransformComponent* transform_component =
            m_parent_object.lock()->tryGetComponent<TransformComponent>("TransformComponent");

        Math::Radian turn_angle_yaw = g_runtime_global_context.m_input_system->m_cursor_delta_yaw;

        unsigned int command = g_runtime_global_context.m_input_system->getGameCommand();

        if (command >= (unsigned int)GameCommand::invalid)
            return;

        calculatedDesiredHorizontalMoveSpeed(command, delta_time);
        calculatedDesiredVerticalMoveSpeed(command, delta_time);
        calculatedDesiredMoveDirection(command, transform_component->getRotation());
        calculateDesiredDisplacement(delta_time);
        calculateTargetPosition(transform_component->getPosition());

        transform_component->setPosition(m_target_position);
    }

    void MotorComponent::calculatedDesiredHorizontalMoveSpeed(unsigned int command, float delta_time)
    {
        bool has_move_command = ((unsigned int)GameCommand::forward | (unsigned int)GameCommand::backward |
                                 (unsigned int)GameCommand::left | (unsigned int)GameCommand::right) &
                                command;
        has_move_command &= ((unsigned int)GameCommand::free_carema & command) == 0;
        bool has_sprint_command = (unsigned int)GameCommand::sprint & command;

        bool  is_acceleration    = false;
        float final_acceleration = m_motor_res.m_move_acceleration;
        float min_speed_ratio    = 0.f;
        float max_speed_ratio    = 0.f;
        if (has_move_command)
        {
            is_acceleration = true;
            max_speed_ratio = m_motor_res.m_max_move_speed_ratio;
            if (m_move_speed_ratio >= m_motor_res.m_max_move_speed_ratio)
            {
                final_acceleration = m_motor_res.m_sprint_acceleration;
                is_acceleration    = has_sprint_command;
                min_speed_ratio    = m_motor_res.m_max_move_speed_ratio;
                max_speed_ratio    = m_motor_res.m_max_sprint_speed_ratio;
            }
        }
        else
        {
            is_acceleration = false;
            min_speed_ratio = 0.f;
            max_speed_ratio = m_motor_res.m_max_sprint_speed_ratio;
        }

        m_move_speed_ratio += (is_acceleration ? 1.0f : -1.0f) * final_acceleration * delta_time;
        m_move_speed_ratio = std::clamp(m_move_speed_ratio, min_speed_ratio, max_speed_ratio);
    }

    void MotorComponent::calculatedDesiredVerticalMoveSpeed(unsigned int command, float delta_time)
    {
        std::shared_ptr<PhysicsScene> physics_scene =
            g_runtime_global_context.m_world_manager->getCurrentActivePhysicsScene().lock();
        ASSERT(physics_scene);

        if (m_motor_res.m_jump_height == 0.f)
            return;

        float const gravity = physics_scene->getGravity().length();

        if (m_jump_state == JumpState::idle)
        {
            if ((unsigned int)GameCommand::jump & command)
            {
                m_jump_state                  = JumpState::rising;
                m_vertical_move_speed         = Math::sqrt(m_motor_res.m_jump_height * 2 * gravity);
                m_jump_horizontal_speed_ratio = m_move_speed_ratio;
            }
            else
            {
                m_vertical_move_speed = 0.f;
            }
        }
        else if (m_jump_state == JumpState::rising || m_jump_state == JumpState::falling)
        {
            m_vertical_move_speed -= gravity * delta_time;
            if (m_vertical_move_speed <= 0.f)
            {
                m_jump_state = JumpState::falling;
            }
        }
    }

    void MotorComponent::calculatedDesiredMoveDirection(unsigned int command, Math::Quaternion const& object_rotation)
    {
        if (m_jump_state == JumpState::idle)
        {
            Math::Vec3 forward_dir = object_rotation * Math::Vec3::NEGATIVE_UNIT_Y;
            Math::Vec3 left_dir    = object_rotation * Math::Vec3::UNIT_X;

            if (command > 0)
            {
                m_desired_horizontal_move_direction = Math::Vec3::ZERO;
            }

            if ((unsigned int)GameCommand::forward & command)
            {
                m_desired_horizontal_move_direction += forward_dir;
            }

            if ((unsigned int)GameCommand::backward & command)
            {
                m_desired_horizontal_move_direction -= forward_dir;
            }

            if ((unsigned int)GameCommand::left & command)
            {
                m_desired_horizontal_move_direction += left_dir;
            }

            if ((unsigned int)GameCommand::right & command)
            {
                m_desired_horizontal_move_direction -= left_dir;
            }

            m_desired_horizontal_move_direction.normalise();
        }
    }

    void MotorComponent::calculateDesiredDisplacement(float delta_time)
    {
        float horizontal_speed_ratio =
            m_jump_state == JumpState::idle ? m_move_speed_ratio : m_jump_horizontal_speed_ratio;
        m_desired_displacement =
            m_desired_horizontal_move_direction * m_motor_res.m_move_speed * horizontal_speed_ratio * delta_time +
            Math::Vec3::UNIT_Z * m_vertical_move_speed * delta_time;
    }

    void MotorComponent::calculateTargetPosition(Math::Vec3 const&& current_position)
    {
        Math::Vec3 final_position;

        switch (m_controller_type)
        {
            case ControllerType::none:
                final_position = current_position + m_desired_displacement;
                break;
            case ControllerType::physics:
                final_position = m_controller->move(current_position, m_desired_displacement);
                break;
            default:
                final_position = current_position;
                break;
        }

        // Piccolo-hack: motor level simulating jump, character always above z-plane
        if (m_jump_state == JumpState::falling && final_position.z + m_desired_displacement.z <= 0.f)
        {
            final_position.z = 0.f;
            m_jump_state     = JumpState::idle;
        }

        m_is_moving       = (final_position - current_position).squaredLength() > 0.f;
        m_target_position = final_position;
    }

} // namespace MarsEngine

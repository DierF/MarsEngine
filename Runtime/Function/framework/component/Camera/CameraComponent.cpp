#include "Runtime/Function/Framework/Component/Camera/CameraComponent.h"
#include "Runtime/Function/Framework/Component/Transform/TransformComponent.h"
#include "Runtime/Function/Framework/Level/Level.h"
#include "Runtime/Function/Framework/Object/Object.h"
#include "Runtime/Function/Framework/World/WorldManager.h"
#include "Runtime/Function/Character/Character.h"
#include "Runtime/Function/Global/GlobalContext.h"
#include "Runtime/Function/Input/InputSystem.h"
#include "Runtime/Function/Render/RenderCamera.h"
#include "Runtime/Function/Render/RenderSwapContext.h"
#include "Runtime/Function/Render/RenderSystem.h"
#include "Runtime/Core/Base/Macro.h"
#include "Runtime/Core/Math/MathHeaders.h"

namespace MarsEngine
{
    void CameraComponent::postLoadResource(std::weak_ptr<GObject> parent_object)
    {
        m_parent_object = parent_object;

        std::string const& camera_type_name = m_camera_res.m_parameter.getTypeName();
        if (camera_type_name == "FirstPersonCameraParameter")
        {
            m_camera_mode = CameraMode::first_person;
        }
        else if (camera_type_name == "ThirdPersonCameraParameter")
        {
            m_camera_mode = CameraMode::third_person;
        }
        else if (camera_type_name == "FreeCameraParameter")
        {
            m_camera_mode = CameraMode::free;
        }
        else
        {
            LOG_ERROR("invalid camera type");
        }

        RenderSwapContext& swap_context = g_runtime_global_context.m_render_system->getSwapContext();
        CameraSwapData     camera_swap_data;
        camera_swap_data.m_fov_x                           = m_camera_res.m_parameter->m_fov;
        swap_context.getLogicSwapData().m_camera_swap_data = camera_swap_data;
    }

    void CameraComponent::tick(float delta_time)
    {
        if (!m_parent_object.lock())
            return;

        std::shared_ptr<Level> current_level = g_runtime_global_context.m_world_manager->getCurrentActiveLevel().lock();
        std::shared_ptr<Character> current_character = current_level->getCurrentActiveCharacter().lock();
        if (current_character == nullptr)
            return;

        if (current_character->getObjectID() != m_parent_object.lock()->getID())
            return;

        switch (m_camera_mode)
        {
            case CameraMode::first_person:
                tickFirstPersonCamera(delta_time);
                break;
            case CameraMode::third_person:
                tickThirdPersonCamera(delta_time);
                break;
            case CameraMode::free:
                tickFreeCamera(delta_time);
                break;
            default:
                break;
        }
    }

    void CameraComponent::tickFirstPersonCamera(float delta_time)
    {
        std::shared_ptr<Level> current_level = g_runtime_global_context.m_world_manager->getCurrentActiveLevel().lock();
        std::shared_ptr<Character> current_character = current_level->getCurrentActiveCharacter().lock();
        if (current_character == nullptr)
            return;

        Math::Quaternion q_yaw, q_pitch;

        q_yaw.fromAngleAxis(g_runtime_global_context.m_input_system->m_cursor_delta_yaw, Math::Vec3::UNIT_Z);
        q_pitch.fromAngleAxis(g_runtime_global_context.m_input_system->m_cursor_delta_pitch, m_left);

        float const offset  = static_cast<FirstPersonCameraParameter*>(m_camera_res.m_parameter)->m_vertical_offset;
        m_position = current_character->getPosition() + offset * Math::Vec3::UNIT_Z;

        m_forward = q_yaw * q_pitch * m_forward;
        m_left    = q_yaw * q_pitch * m_left;
        m_up      = m_forward.crossProduct(m_left);

        Math::Mat4 desired_mat = Math::makeLookAtMatrix(m_position, m_position + m_forward, m_up);

        RenderSwapContext& swap_context = g_runtime_global_context.m_render_system->getSwapContext();
        CameraSwapData     camera_swap_data;
        camera_swap_data.m_camera_type                     = RenderCameraType::Motor;
        camera_swap_data.m_view_matrix                     = desired_mat;
        swap_context.getLogicSwapData().m_camera_swap_data = camera_swap_data;

        Math::Vec3    object_facing = m_forward - m_forward.dotProduct(Math::Vec3::UNIT_Z) * Math::Vec3::UNIT_Z;
        Math::Vec3    object_left   = Math::Vec3::UNIT_Z.crossProduct(object_facing);
        Math::Quaternion object_rotation;
        object_rotation.fromAxes(object_left, -object_facing, Math::Vec3::UNIT_Z);
        current_character->setRotation(object_rotation);
    }

    void CameraComponent::tickThirdPersonCamera(float delta_time)
    {
        std::shared_ptr<Level> current_level = g_runtime_global_context.m_world_manager->getCurrentActiveLevel().lock();
        std::shared_ptr<Character> current_character = current_level->getCurrentActiveCharacter().lock();
        if (current_character == nullptr)
            return;

        ThirdPersonCameraParameter* param = static_cast<ThirdPersonCameraParameter*>(m_camera_res.m_parameter);

        Math::Quaternion q_yaw, q_pitch;

        q_yaw.fromAngleAxis(g_runtime_global_context.m_input_system->m_cursor_delta_yaw, Math::Vec3::UNIT_Z);
        q_pitch.fromAngleAxis(g_runtime_global_context.m_input_system->m_cursor_delta_pitch, Math::Vec3::UNIT_X);

        param->m_cursor_pitch = q_pitch * param->m_cursor_pitch;

        float const vertical_offset   = param->m_vertical_offset;
        float const horizontal_offset = param->m_horizontal_offset;
        Math::Vec3     offset            = Math::Vec3(0, horizontal_offset, vertical_offset);

        Math::Vec3 center_pos = current_character->getPosition() + Math::Vec3::UNIT_Z * vertical_offset;
        m_position =
            current_character->getRotation() * param->m_cursor_pitch * offset + current_character->getPosition();

        m_forward = center_pos - m_position;
        m_up = current_character->getRotation() * param->m_cursor_pitch * Math::Vec3::UNIT_Z;
        m_left = m_up.crossProduct(m_forward);

        current_character->setRotation(q_yaw * current_character->getRotation());

        Math::Mat4 desired_mat = Math::makeLookAtMatrix(m_position, m_position + m_forward, m_up);

        RenderSwapContext& swap_context = g_runtime_global_context.m_render_system->getSwapContext();
        CameraSwapData     camera_swap_data;
        camera_swap_data.m_camera_type                     = RenderCameraType::Motor;
        camera_swap_data.m_view_matrix                     = desired_mat;
        swap_context.getLogicSwapData().m_camera_swap_data = camera_swap_data;
    }

    void CameraComponent::tickFreeCamera(float delta_time)
    {
        unsigned int command = g_runtime_global_context.m_input_system->getGameCommand();
        if (command >= (unsigned int)GameCommand::invalid) return;

        std::shared_ptr<Level> current_level = g_runtime_global_context.m_world_manager->getCurrentActiveLevel().lock();
        std::shared_ptr<Character> current_character = current_level->getCurrentActiveCharacter().lock();
        if (current_character == nullptr)
            return;

        Math::Quaternion q_yaw, q_pitch;

        q_yaw.fromAngleAxis(g_runtime_global_context.m_input_system->m_cursor_delta_yaw, Math::Vec3::UNIT_Z);
        q_pitch.fromAngleAxis(g_runtime_global_context.m_input_system->m_cursor_delta_pitch, m_left);

        m_forward = q_yaw * q_pitch * m_forward;
        m_left = q_yaw * q_pitch * m_left;
        m_up = m_forward.crossProduct(m_left);

        bool has_move_command = ((unsigned int)GameCommand::forward | (unsigned int)GameCommand::backward |
                                 (unsigned int)GameCommand::left | (unsigned int)GameCommand::right) & command;
        if (has_move_command)
        {
            Math::Vec3 move_direction = Math::Vec3::ZERO;

            if ((unsigned int)GameCommand::forward & command)
            {
                move_direction += m_forward;
            }

            if ((unsigned int)GameCommand::backward & command)
            {
                move_direction -= m_forward;
            }

            if ((unsigned int)GameCommand::left & command)
            {
                move_direction += m_left;
            }

            if ((unsigned int)GameCommand::right & command)
            {
                move_direction -= m_left;
            }

            m_position += move_direction * 2.0f * delta_time;
        }

        Math::Mat4 desired_mat = Math::makeLookAtMatrix(m_position, m_position + m_forward, m_up);

        RenderSwapContext& swap_context = g_runtime_global_context.m_render_system->getSwapContext();
        CameraSwapData     camera_swap_data;
        camera_swap_data.m_camera_type = RenderCameraType::Motor;
        camera_swap_data.m_view_matrix = desired_mat;
        swap_context.getLogicSwapData().m_camera_swap_data = camera_swap_data;
    }
} // namespace MarsEngine

#include "Runtime/Function/Character/Character.h"
#include "Runtime/Function/Framework/Component/Motor/MotorComponent.h"
#include "Runtime/Function/Framework/Component/Transform/TransformComponent.h"
#include "Runtime/Function/Global/GlobalContext.h"
#include "Runtime/Function/Input/InputSystem.h"
#include "Runtime/Engine.h"

namespace MarsEngine
{
    Character::Character(std::shared_ptr<GObject> character_object) { setObject(character_object); }

    GObjectID Character::getObjectID() const
    {
        if (m_character_object)
        {
            return m_character_object->getID();
        }

        return k_invalid_gobject_id;
    }

    void Character::setObject(std::shared_ptr<GObject> gobject)
    {
        m_character_object = gobject;
        if (m_character_object)
        {
            TransformComponent const* transform_component =
                m_character_object->tryGetComponentConst(TransformComponent);
            Math::Transform const& transform = transform_component->getTransformConst();
            m_position                 = transform.m_position;
            m_rotation                 = transform.m_rotation;
        }
        else
        {
            m_position = Math::Vec3::ZERO;
            m_rotation = Math::Quaternion::IDENTITY;
        }
    }

    void Character::tick(float delta_time)
    {
        if (m_character_object == nullptr)
            return;

        unsigned int command = g_runtime_global_context.m_input_system->getGameCommand();
        if (command < (unsigned int)GameCommand::invalid)
        {
            if ((((unsigned int)GameCommand::free_carema & command) > 0) != m_is_free_camera)
            {
                toggleFreeCamera();
            }
        }

        TransformComponent* transform_component = m_character_object->tryGetComponent(TransformComponent);

        if (m_rotation_dirty)
        {
            transform_component->setRotation(m_rotation_buffer);
            m_rotation_dirty = false;
        }

        MotorComponent const* motor_component = m_character_object->tryGetComponentConst(MotorComponent);
        if (motor_component == nullptr)
        {
            return;
        }

        if (motor_component->getIsMoving())
        {
            m_rotation_buffer = m_rotation;
            transform_component->setRotation(m_rotation_buffer);
            m_rotation_dirty = true;
        }

        Math::Vec3 const& new_position = motor_component->getTargetPosition();

        m_position = new_position;

        //float blend_ratio = std::max(1.f, motor_component->getSpeedRatio());

        //float frame_length = delta_time * blend_ratio;
        //m_position =
        //    (m_position * (s_camera_blend_time - frame_length) + new_position * frame_length) / s_camera_blend_time;
        //m_position =
        //    (m_position * (s_camera_blend_time - frame_length) + new_position * frame_length) / s_camera_blend_time;
    }

    void Character::toggleFreeCamera()
    {
        CameraComponent* camera_component = m_character_object->tryGetComponent(CameraComponent);
        if (camera_component == nullptr) return;

        m_is_free_camera = !m_is_free_camera;

        if (m_is_free_camera)
        {
            m_original_camera_mode = camera_component->getCameraMode();
            camera_component->setCameraMode(CameraMode::free);
        }
        else
        {
            camera_component->setCameraMode(m_original_camera_mode);
        }
    }
} // namespace MarsEngine
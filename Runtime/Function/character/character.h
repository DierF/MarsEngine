#pragma once

#include "Runtime/Core/Math/Transform.h"
#include "Runtime/Function/Framework/Component/Camera/CameraComponent.h"
#include "Runtime/Function/Framework/Object/Object.h"

#include <vector>

namespace MarsEngine
{
    class Character
    {
        inline static float const s_camera_blend_time {0.3f};

    public:
        Character(std::shared_ptr<GObject> character_object);

        GObjectID getObjectID() const;
        void      setObject(std::shared_ptr<GObject> gobject);
        std::weak_ptr<GObject> getObject() const { return m_character_object; }

        void setPosition(Vec3 const& position) { m_position = position; }
        void setRotation(Quaternion const& rotation) { m_rotation = rotation; }

        Vec3 const&    getPosition() const { return m_position; }
        Quaternion const& getRotation() const { return m_rotation; }

        void tick(float delta_time);

    private:
        void toggleFreeCamera();

        Vec3       m_position;
        Quaternion m_rotation;

        std::shared_ptr<GObject> m_character_object;

        // hack for setting rotation frame buffer
        Quaternion m_rotation_buffer;
        bool       m_rotation_dirty {false};

        CameraMode m_original_camera_mode;
        bool       m_is_free_camera{false};
    };
} // namespace MarsEngine
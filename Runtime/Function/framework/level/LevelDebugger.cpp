#include "Runtime/Function/Framework/Level/LevelDebugger.h"
#include "Runtime/Function/Framework/Component/Component.h"
#include "Runtime/Function/Framework/Component/Animation/AnimationComponent.h"
#include "Runtime/Function/Framework/Component/Transform/TransformComponent.h"
#include "Runtime/Function/Framework/Component/Camera/CameraComponent.h"
#include "Runtime/Function/Framework/Component/Rigidbody/RigidbodyComponent.h"
#include "Runtime/Function/Character/Character.h"
#include "Runtime/Function/Global/GlobalContext.h"
#include "Runtime/Function/Render/DebugDraw/DebugDrawManager.h"
#include "Runtime/Function/Render/RenderDebugConfig.h"
#include "Runtime/Core/Meta/Reflection/Reflection.h"
#include "Runtime/Core/Math/AxisAligned.h"
#include "Runtime/Resource/AssetManager/AssetManager.h"
#include "Runtime/Resource/ResType/Components/Animation.h"
#include "Runtime/Engine.h"

namespace MarsEngine
{
    void LevelDebugger::tick(std::shared_ptr<Level> level) const
    {
        if (g_is_editor_mode)
        {
            return;
        }

        if (g_runtime_global_context.m_render_debug_config->animation.show_bone_name)
        {
            showAllBonesName(level);
        }
        if (g_runtime_global_context.m_render_debug_config->animation.show_skeleton)
        {
            showAllBones(level);
        }
        if (g_runtime_global_context.m_render_debug_config->gameObject.show_bounding_box)
        {
            showAllBoundingBox(level);
        }
        if (g_runtime_global_context.m_render_debug_config->camera.show_runtime_info)
        {
            showCameraInfo(level);
        }
    }

    void LevelDebugger::showAllBones(std::shared_ptr<Level> level) const
    {
        LevelObjectsMap const& go_map = level->getAllGObjects();
        for (auto const& gobject_pair : go_map)
        {
            drawBones(gobject_pair.second);
        }
    }

    void LevelDebugger::showBones(std::shared_ptr<Level> level, GObjectID go_id) const
    {
        std::shared_ptr<GObject> gobject = level->getGObjectByID(go_id).lock();
        drawBones(gobject);
    }

    void LevelDebugger::showAllBonesName(std::shared_ptr<Level> level) const
    {
        LevelObjectsMap const& go_map = level->getAllGObjects();
        for (auto const& gobject_pair : go_map)
        {
            drawBonesName(gobject_pair.second);
        }
    }

    void LevelDebugger::showBonesName(std::shared_ptr<Level> level, GObjectID go_id) const
    {
        std::shared_ptr<GObject> gobject = level->getGObjectByID(go_id).lock();
        drawBonesName(gobject);
    }

    void LevelDebugger::showAllBoundingBox(std::shared_ptr<Level> level) const
    {
        LevelObjectsMap const& go_map = level->getAllGObjects();
        for (auto const& gobject_pair : go_map)
        {
            drawBoundingBox(gobject_pair.second);
        }
    }

    void LevelDebugger::showBoundingBox(std::shared_ptr<Level> level, GObjectID go_id) const
    {
        std::shared_ptr<GObject> gobject = level->getGObjectByID(go_id).lock();
        drawBoundingBox(gobject);
    }

    void LevelDebugger::showCameraInfo(std::shared_ptr<Level> level) const
    {
        std::shared_ptr<GObject> gobject = level->getCurrentActiveCharacter().lock()->getObject().lock();
        drawCameraInfo(gobject);
    }
    void LevelDebugger::drawBones(std::shared_ptr<GObject> object) const
    {
        TransformComponent const* transform_component =
            object->tryGetComponentConst<TransformComponent>("TransformComponent");
        AnimationComponent const* animation_component =
            object->tryGetComponentConst<AnimationComponent>("AnimationComponent");

        if (transform_component == nullptr || animation_component == nullptr)
            return;

        DebugDrawGroup* debug_draw_group =
            g_runtime_global_context.m_debugdraw_manager->tryGetOrCreateDebugDrawGroup("bone");

        Mat4 object_matrix = Transform(transform_component->getPosition(),
                                            transform_component->getRotation(),
                                            transform_component->getScale())
                                      .getMatrix();

        Skeleton const& skeleton    = animation_component->getSkeleton();
        Bone const*     bones       = skeleton.getBones();
        int32_t         bones_count = skeleton.getBonesCount();
        for (int32_t bone_index = 0; bone_index < bones_count; bone_index++)
        {
            if (bones[bone_index].getParent() == nullptr || bone_index == 1)
                continue;

            Mat4 bone_matrix = Transform(bones[bone_index]._getDerivedPosition(),
                                              bones[bone_index]._getDerivedOrientation(),
                                              bones[bone_index]._getDerivedScale())
                                        .getMatrix();
            Vec4 bone_position(0.0f, 0.0f, 0.0f, 1.0f);
            bone_position = object_matrix * bone_matrix * bone_position;
            bone_position /= bone_position[3];

            Node*     parent_bone        = bones[bone_index].getParent();
            Mat4 parent_bone_matrix = Transform(parent_bone->_getDerivedPosition(),
                                                     parent_bone->_getDerivedOrientation(),
                                                     parent_bone->_getDerivedScale())
                                               .getMatrix();
            Vec4 parent_bone_position(0.0f, 0.0f, 0.0f, 1.0f);
            parent_bone_position = object_matrix * parent_bone_matrix * parent_bone_position;
            parent_bone_position /= parent_bone_position[3];

            debug_draw_group->addLine(Vec3(bone_position.x, bone_position.y, bone_position.z),
                                      Vec3(parent_bone_position.x, parent_bone_position.y, parent_bone_position.z),
                                      Vec4(1.0f, 0.0f, 0.0f, 1.0f),
                                      Vec4(1.0f, 0.0f, 0.0f, 1.0f),
                                      0.0f,
                                      true);
            debug_draw_group->addSphere(Vec3(bone_position.x, bone_position.y, bone_position.z),
                                        0.015f,
                                        Vec4(0.0f, 0.0f, 1.0f, 1.0f),
                                        0.0f,
                                        true);
        }
    }

    void LevelDebugger::drawBonesName(std::shared_ptr<GObject> object) const
    {
        TransformComponent const* transform_component =
            object->tryGetComponentConst<TransformComponent>("TransformComponent");
        AnimationComponent const* animation_component =
            object->tryGetComponentConst<AnimationComponent>("AnimationComponent");

        if (transform_component == nullptr || animation_component == nullptr)
            return;

        DebugDrawGroup* debug_draw_group =
            g_runtime_global_context.m_debugdraw_manager->tryGetOrCreateDebugDrawGroup("bone name");

        Mat4 object_matrix = Transform(transform_component->getPosition(),
                                            transform_component->getRotation(),
                                            transform_component->getScale())
                                      .getMatrix();

        Skeleton const& skeleton    = animation_component->getSkeleton();
        Bone const*     bones       = skeleton.getBones();
        int32_t         bones_count = skeleton.getBonesCount();
        for (int32_t bone_index = 0; bone_index < bones_count; bone_index++)
        {
            if (bones[bone_index].getParent() == nullptr || bone_index == 1)
                continue;

            Mat4 bone_matrix = Transform(bones[bone_index]._getDerivedPosition(),
                                              bones[bone_index]._getDerivedOrientation(),
                                              bones[bone_index]._getDerivedScale())
                                        .getMatrix();
            Vec4 bone_position(0.0f, 0.0f, 0.0f, 1.0f);
            bone_position = object_matrix * bone_matrix * bone_position;
            bone_position /= bone_position[3];

            debug_draw_group->addText(bones[bone_index].getName(),
                                      Vec4(1.0f, 0.0f, 0.0f, 1.0f),
                                      Vec3(bone_position.x, bone_position.y, bone_position.z),
                                      8,
                                      false);
        }
    }

    void LevelDebugger::drawBoundingBox(std::shared_ptr<GObject> object) const
    {
        RigidBodyComponent const* rigidbody_component =
            object->tryGetComponentConst<RigidBodyComponent>("RigidBodyComponent");
        if (rigidbody_component == nullptr)
            return;

        std::vector<AxisAlignedBox> bounding_boxes;
        rigidbody_component->getShapeBoundingBoxes(bounding_boxes);
        for (size_t bounding_box_index = 0; bounding_box_index < bounding_boxes.size(); bounding_box_index++)
        {
            AxisAlignedBox  bounding_box = bounding_boxes[bounding_box_index];
            DebugDrawGroup* debug_draw_group =
                g_runtime_global_context.m_debugdraw_manager->tryGetOrCreateDebugDrawGroup("bounding box");
            Vec3 center =
                Vec3(bounding_box.getCenter().x, bounding_box.getCenter().y, bounding_box.getCenter().z);
            Vec3 halfExtent =
                Vec3(bounding_box.getHalfExtent().x, bounding_box.getHalfExtent().y, bounding_box.getHalfExtent().z);

            debug_draw_group->addBox(
                center, halfExtent, Vec4(1.0f, 0.0f, 0.0f, 0.0f), Vec4(0.0f, 1.0f, 0.0f, 1.0f));
        }
    }

    void LevelDebugger::drawCameraInfo(std::shared_ptr<GObject> object) const
    {
        CameraComponent const* camera_component = object->tryGetComponentConst<CameraComponent>("CameraComponent");
        if (camera_component == nullptr)
            return;

        DebugDrawGroup* debug_draw_group =
            g_runtime_global_context.m_debugdraw_manager->tryGetOrCreateDebugDrawGroup("show camera info");

        std::ostringstream buffer;
        buffer << "camera mode: ";
        switch (camera_component->getCameraMode())
        {
            case CameraMode::first_person:
                buffer << "first person";
                break;
            case CameraMode::third_person:
                buffer << "third person";
                break;
            case CameraMode::free:
                buffer << "free";
                break;
            case CameraMode::invalid:
                buffer << "invalid";
                break;
        }
        buffer << std::endl;

        Vec3 position  = camera_component->getPosition();
        Vec3 forward   = camera_component->getForward();
        Vec3 direction = forward - position;
        buffer << "camera position: (" << position.x << "," << position.y << "," << position.z << ")" << std::endl;
        buffer << "camera direction : (" << direction.x << "," << direction.y << "," << direction.z << ")";
        debug_draw_group->addText(buffer.str(), Vec4(1.0f, 0.0f, 0.0f, 1.0f), Vec3(-1.0f, -0.2f, 0.0f), 10, true);
    }
} // namespace MarsEngine
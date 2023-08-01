#pragma once

#include "Editor/Include/Axis.h"
#include "Runtime/Core/Math/Vector2.h"
#include "Runtime/Function/Framework/Object/Object.h"
#include "Runtime/Function/Render/RenderObject.h"

#include <memory>

namespace MarsEngine
{
    class MarsEditor;
    class RenderCamera;
    class RenderEntity;

    enum class EditorAxisMode : int
    {
        TranslateMode = 0,
        RotateMode = 1,
        ScaleMode = 2,
        Default = 3
    };

    class EditorSceneManager
    {
    public:
        void initialize();
        void tick(float delta_time);

    public:
        size_t   updateCursorOnAxis(Vec2 cursor_uv,
                                    Vec2 game_engine_window_size);
        void drawSelectedEntityAxis();
        std::weak_ptr<GObject> getSelectedGObject() const;
        RenderEntity* getAxisMeshByType(EditorAxisMode axis_mode);
        void onGObjectSelected(GObjectID selected_gobject_id);
        void onDeleteSelectedGObject();
        void moveEntity(float      new_mouse_pos_x,
                        float      new_mouse_pos_y,
                        float      last_mouse_pos_x,
                        float      last_mouse_pos_y,
                        Vec2 engine_window_pos,
                        Vec2 engine_window_size,
                        size_t     cursor_on_axis,
                        Mat4 model_matrix);

        void setEditorCamera(std::shared_ptr<RenderCamera> camera) { m_camera = camera; }
        void uploadAxisResource();
        size_t getGuidOfPickedMesh(Vec2 const& picked_uv) const;

    public:
        std::shared_ptr<RenderCamera> getEditorCamera() { return m_camera; };

        GObjectID getSelectedObjectID() { return m_selected_gobject_id; };
        Mat4 getSelectedObjectMatrix() { return m_selected_object_matrix; }
        EditorAxisMode getEditorAxisMode() { return m_axis_mode; }

        void setSelectedObjectID(GObjectID selected_gobject_id) { m_selected_gobject_id = selected_gobject_id; };
        void setSelectedObjectMatrix(Mat4 new_object_matrix) { m_selected_object_matrix = new_object_matrix; }
        void setEditorAxisMode(EditorAxisMode new_axis_mode) { m_axis_mode = new_axis_mode; }
    private:
        EditorTranslationAxis m_translation_axis;
        EditorRotationAxis    m_rotation_axis;
        EditorScaleAxis       m_scale_aixs;

        GObjectID m_selected_gobject_id{ k_invalid_gobject_id };
        Mat4 m_selected_object_matrix{ Mat4::IDENTITY };

        EditorAxisMode m_axis_mode{ EditorAxisMode::TranslateMode };
        std::shared_ptr<RenderCamera> m_camera;

        size_t m_selected_axis{ 3 };

        bool m_is_show_axis = true;
    };
} // namespace MarsEngine
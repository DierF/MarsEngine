#pragma once

namespace MarsEngine
{
    struct EditorGlobalContextInitInfo
    {
        class WindowSystem* window_system;
        class RenderSystem* render_system;
        class MarsEngine*   engine_runtime;
    };

    class EditorGlobalContext
    {
    public:
        class EditorSceneManager* m_scene_manager {nullptr};
        class EditorInputManager* m_input_manager {nullptr};
        class RenderSystem*       m_render_system {nullptr};
        class WindowSystem*       m_window_system {nullptr};
        class MarsEngine*         m_engine_runtime {nullptr};

    public:
        void initialize(EditorGlobalContextInitInfo const& init_info);
        void clear();
    };

    extern EditorGlobalContext g_editor_global_context;
} // namespace MarsEngine
#include "Editor/Include/EditorGlobalContext.h"
#include "Editor/Include/EditorInputManager.h"
#include "Editor/Include/EditorSceneManager.h"
#include "Runtime/Function/Render/RenderSystem.h"
#include "Runtime/Function/Render/WindowSystem.h"

namespace MarsEngine
{
    EditorGlobalContext g_editor_global_context;

    void EditorGlobalContext::initialize(EditorGlobalContextInitInfo const& init_info)
    {
        g_editor_global_context.m_window_system  = init_info.window_system;
        g_editor_global_context.m_render_system  = init_info.render_system;
        g_editor_global_context.m_engine_runtime = init_info.engine_runtime;

        m_scene_manager = new EditorSceneManager();
        m_input_manager = new EditorInputManager();
        m_scene_manager->initialize();
        m_input_manager->initialize();
    }

    void EditorGlobalContext::clear()
    {
        delete (m_scene_manager);
        delete (m_input_manager);
    }
} // namespace MarsEngine
#include "Runtime/Function/Physics/PhysicsManager.h"
#include "Runtime/Function/Framework/World/WorldManager.h"
#include "Runtime/Function/Global/GlobalContext.h"
#include "Runtime/Function/Physics/Jolt/Utils.h"
#include "Runtime/Function/Physics/PhysicsScene.h"
#include "Runtime/Function/Render/RenderSystem.h"
#include "Runtime/Resource/ConfigManager/ConfigManager.h"

#ifdef ENABLE_PHYSICS_DEBUG_RENDERER
#include "TestFramework.h"

#include "TestFramework/Renderer/DebugRendererImp.h"
#include "TestFramework/Renderer/Font.h"
#include "TestFramework/Renderer/Renderer.h"
#include "TestFramework/Utils/Log.h"
#endif

namespace MarsEngine
{
    void PhysicsManager::initialize()
    {
#ifdef ENABLE_PHYSICS_DEBUG_RENDERER
        std::shared_ptr<ConfigManager> config_manager = g_runtime_global_context.m_config_manager;
        ASSERT(config_manager);

        Trace = TraceImpl;

        m_renderer = new Renderer;
        m_renderer->Initialize();

        m_font = new Font(m_renderer);
        m_font->Create("Arial", 24, config_manager->getJoltPhysicsAssetFolder());

        m_debug_renderer = new DebugRendererImp(m_renderer, m_font, config_manager->getJoltPhysicsAssetFolder());
#endif
    }

    void PhysicsManager::clear()
    {
        m_scenes.clear();

#ifdef ENABLE_PHYSICS_DEBUG_RENDERER
        delete m_debug_renderer;
        m_font = nullptr;
        delete m_renderer;
#endif
    }

    std::weak_ptr<PhysicsScene> PhysicsManager::createPhysicsScene(Math::Vec3 const& gravity)
    {
        std::shared_ptr<PhysicsScene> physics_scene = std::make_shared<PhysicsScene>(gravity);

        m_scenes.push_back(physics_scene);

        return physics_scene;
    }

    void PhysicsManager::deletePhysicsScene(std::weak_ptr<PhysicsScene> physics_scene)
    {
        std::shared_ptr<PhysicsScene> deleted_scene = physics_scene.lock();

        auto iter = std::find(m_scenes.begin(), m_scenes.end(), deleted_scene);
        if (iter != m_scenes.end())
        {
            m_scenes.erase(iter);
        }
    }

#ifdef ENABLE_PHYSICS_DEBUG_RENDERER
    void PhysicsManager::renderPhysicsWorld(float delta_time)
    {
        std::shared_ptr<PhysicsScene> physics_scene =
            g_runtime_global_context.m_world_manager->getCurrentActivePhysicsScene().lock();

        std::shared_ptr<RenderCamera> render_camera = g_runtime_global_context.m_render_system->getRenderCamera();
        Vector2 const&                fov           = render_camera->getFOV();

        EngineContentViewport const& engine_viewport =
            g_runtime_global_context.m_render_system->getEngineContentViewport();

        HWND window_handle = m_renderer->GetWindowHandle();
        RECT rc;
        GetClientRect(window_handle, &rc);

        float const current_width  = static_cast<float>(rc.right - rc.left);
        float const current_height = static_cast<float>(rc.bottom - rc.top);

        if (!Math::realEqual(engine_viewport.width, current_width, 1e-1) ||
            !Math::realEqual(engine_viewport.height, current_height, 1e-1))
        {
            ::SetWindowPos(window_handle,
                           NULL,
                           rc.left,
                           rc.top,
                           engine_viewport.width,
                           engine_viewport.height,
                           SWP_NOMOVE | SWP_NOACTIVATE | SWP_DEFERERASE | SWP_NOOWNERZORDER);
        }

        CameraState world_camera;
        world_camera.mPos       = toVec3(render_camera->position());
        world_camera.mForward   = toVec3(render_camera->forward());
        world_camera.mUp        = toVec3(render_camera->up());
        world_camera.mFOVY      = fov.y;
        world_camera.mFarPlane  = render_camera->m_zfar;
        world_camera.mNearPlane = render_camera->m_znear;

        m_renderer->BeginFrame(world_camera, 1.f);

        physics_scene->drawPhysicsScene(m_debug_renderer);

        static_cast<DebugRendererImp*>(m_debug_renderer)->Draw();
        static_cast<DebugRendererImp*>(m_debug_renderer)->Clear();

        m_renderer->EndFrame();
    }
#endif
} // namespace MarsEngine
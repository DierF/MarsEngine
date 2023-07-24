#include "Runtime/Function/Global/GlobalContext.h"
#include "Runtime/Function/Framework/World/WorldManager.h"
#include "Runtime/Function/Input/InputSystem.h"
#include "Runtime/Function/Particle/ParticleManager.h"
#include "Runtime/Function/Physics/PhysicsManager.h"
#include "Runtime/Function/Render/DebugDraw/DebugDrawManager.h"
#include "Runtime/Function/Render/RenderDebugConfig.h"
#include "Runtime/Function/Render/RenderSystem.h"
#include "Runtime/Function/Render/WindowSystem.h"
#include "Runtime/Platform/FileService/FileService.h"
#include "Runtime/Resource/AssetManager/AssetManager.h"
#include "Runtime/Resource/ConfigManager/ConfigManager.h"
#include "Runtime/Core/Log/LogSystem.h"
#include "Runtime/Engine.h"

namespace MarsEngine
{
    RuntimeGlobalContext g_runtime_global_context;

    void RuntimeGlobalContext::startSystems(std::string const& config_file_path)
    {
        m_config_manager = std::make_shared<ConfigManager>();
        m_config_manager->initialize(config_file_path);

        m_file_system = std::make_shared<FileSystem>();

        m_logger_system = std::make_shared<LogSystem>();

        m_asset_manager = std::make_shared<AssetManager>();

        m_physics_manager = std::make_shared<PhysicsManager>();
        m_physics_manager->initialize();

        m_world_manager = std::make_shared<WorldManager>();
        m_world_manager->initialize();

        m_window_system = std::make_shared<WindowSystem>();
        WindowCreateInfo window_create_info;
        m_window_system->initialize(window_create_info);

        m_input_system = std::make_shared<InputSystem>();
        m_input_system->initialize();

        m_particle_manager = std::make_shared<ParticleManager>();
        m_particle_manager->initialize();

        m_render_system = std::make_shared<RenderSystem>();
        RenderSystemInitInfo render_init_info;
        render_init_info.window_system = m_window_system;
        m_render_system->initialize(render_init_info);

        m_debugdraw_manager = std::make_shared<DebugDrawManager>();
        m_debugdraw_manager->initialize();

        m_render_debug_config = std::make_shared<RenderDebugConfig>();
    }

    void RuntimeGlobalContext::shutdownSystems()
    {
        m_render_debug_config.reset();

        m_debugdraw_manager.reset();

        m_render_system->clear();
        m_render_system.reset();

        m_window_system.reset();

        m_world_manager->clear();
        m_world_manager.reset();

        m_physics_manager->clear();
        m_physics_manager.reset();

        m_input_system->clear();
        m_input_system.reset();

        m_asset_manager.reset();

        m_logger_system.reset();

        m_file_system.reset();

        m_config_manager.reset();

        m_particle_manager.reset();
    }
} // namespace MarsEngine
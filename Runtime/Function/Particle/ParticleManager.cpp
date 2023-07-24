#include "Runtime/Function/Particle/ParticleManager.h"
#include "Runtime/Function/Particle/EmitterIdAllocator.h"
#include "Runtime/Function/Particle/ParticleCommon.h"
#include "Runtime/Function/Render/Passes/ParticlePass.h"
#include "Runtime/Function/Render/RenderSystem.h"
#include "Runtime/Function/Global/GlobalContext.h"
#include "Runtime/Resource/AssetManager/AssetManager.h"
#include "Runtime/Resource/ConfigManager/ConfigManager.h"

namespace MarsEngine
{
    void ParticleManager::initialize()
    {
        std::shared_ptr<ConfigManager> config_manager = g_runtime_global_context.m_config_manager;
        if (config_manager == nullptr)
        {
            LOG_ERROR("config_manager is nullptr");
            return;
        }
        std::shared_ptr<AssetManager> asset_manager = g_runtime_global_context.m_asset_manager;
        if (asset_manager == nullptr)
        {
            LOG_ERROR("asset_manager is nullptr");
            return;
        }

        GlobalParticleRes  global_particle_res;
        std::string const& global_particle_res_url = config_manager->getGlobalParticleResUrl();
        asset_manager->loadAsset(global_particle_res_url, global_particle_res);

        if (global_particle_res.m_emit_gap < 0)
        {
            global_particle_res.m_emit_gap = s_default_particle_emit_gap;
        }
        if (global_particle_res.m_emit_gap % 2)
        {
            LOG_ERROR("emit_gap should be multiples of 2");
            global_particle_res.m_emit_gap = s_default_particle_emit_gap;
        }
        if (global_particle_res.m_time_step < 1e-6)
        {
            LOG_ERROR("time_step should be lager");
            global_particle_res.m_emit_gap = s_default_particle_time_step;
        }
        if (global_particle_res.m_max_life < global_particle_res.m_time_step)
        {
            LOG_ERROR("max_life should be larger");
            global_particle_res.m_max_life = s_default_particle_life_time * s_default_particle_time_step;
        }
        m_global_particle_res = global_particle_res;
    }

    void ParticleManager::createParticleEmitter(ParticleComponentRes const&   particle_res,
                                                ParticleEmitterTransformDesc& transform_desc)
    {
        RenderSwapContext& swap_context = g_runtime_global_context.m_render_system->getSwapContext();
        RenderSwapData&    swap_data    = swap_context.getLogicSwapData();

        ParticleEmitterDesc desc(particle_res, transform_desc);
        swap_data.addNewParticleEmitter(desc);

        transform_desc.m_id = ParticleEmitterIDAllocator::alloc();
    }

    GlobalParticleRes const& ParticleManager::getGlobalParticleRes() { return m_global_particle_res; }
} // namespace MarsEngine
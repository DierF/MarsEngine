#include "Runtime/Function/Framework/Component/Particle/ParticleComponent.h"
#include "Runtime/Function/Framework/Component/Transform/TransformComponent.h"
#include "Runtime/Function/Global/GlobalContext.h"
#include "Runtime/Function/Particle/ParticleManager.h"
#include "Runtime/Function/Global/GlobalContext.h"
#include "Runtime/Function/Render/RenderSwapContext.h"
#include "Runtime/Function/Render/RenderSystem.h"
#include "Runtime/Core/Base/Macro.h"

namespace MarsEngine
{
    void ParticleComponent::postLoadResource(std::weak_ptr<GObject> parent_object)
    {
        m_parent_object = parent_object;

        std::shared_ptr<ParticleManager> particle_manager = g_runtime_global_context.m_particle_manager;
        ASSERT(particle_manager);

        m_local_transform.makeTransform(
            m_particle_res.m_local_translation, Math::Vec3::UNIT_SCALE, m_particle_res.m_local_rotation);
        computeGlobalTransform();

        particle_manager->createParticleEmitter(m_particle_res, m_transform_desc);
    }

    void ParticleComponent::computeGlobalTransform()
    {
        TransformComponent* transform_component =
            m_parent_object.lock()->tryGetComponent<TransformComponent>("TransformComponent");

        Math::Mat4 global_transform_matrix = transform_component->getMatrix() * m_local_transform;

        Math::Vec3    position, scale;
        Math::Quaternion rotation;

        global_transform_matrix.decomposition(position, scale, rotation);

        memcpy(m_transform_desc.m_position.ptr(), position.ptr(), sizeof(float) * 3);
        rotation.toRotationMatrix(m_transform_desc.m_rotation);
    }

    void ParticleComponent::tick(float delta_time)
    {
        RenderSwapContext& swap_context = g_runtime_global_context.m_render_system->getSwapContext();

        RenderSwapData& logic_swap_data = swap_context.getLogicSwapData();

        logic_swap_data.addTickParticleEmitter(m_transform_desc.m_id);

        TransformComponent* transform_component = m_parent_object.lock()->tryGetComponent(TransformComponent);
        if (transform_component->isDirty())
        {
            computeGlobalTransform();

            logic_swap_data.updateParticleTransform(m_transform_desc);
        }
    }
}; // namespace MarsEngine
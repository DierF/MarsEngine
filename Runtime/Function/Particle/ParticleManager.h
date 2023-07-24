#pragma once

#include "Runtime/Function/Particle/ParticleDesc.h"
#include "Runtime/Resource/ResType/Components/Emitter.h"
#include "Runtime/Resource/ResType/Global/GlobalParticle.h"
#include "Runtime/Core/Math/Matrix4.h"
#include "Runtime/Core/Math/Vector4.h"

#include <memory>

namespace MarsEngine
{
    class ParticlePass;
    class ParticleManager
    {
    public:
        ParticleManager() = default;

        ~ParticleManager() {};

        void initialize();
        void clear();

        void setParticlePass(ParticlePass* particle_pass);

        const GlobalParticleRes& getGlobalParticleRes();

        void createParticleEmitter(ParticleComponentRes const&   particle_res,
                                   ParticleEmitterTransformDesc& transform_desc);

    private:
        GlobalParticleRes m_global_particle_res;
    };
} // namespace MarsEngine
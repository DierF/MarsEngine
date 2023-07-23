#pragma once

#include "Runtime/Function/Framework/Component/Component.h"
#include "Runtime/Function/Particle/ParticleDesc.h"
#include "Runtime/Resource/ResType/Components/Emitter.h"
#include "Runtime/Core/Math/Matrix4.h"
#include "Runtime/Core/Math/Transform.h"

namespace MarsEngine
{
    REFLECTION_TYPE(ParticleComponent)
    CLASS(ParticleComponent : public Component, WhiteListFields)
    {
        REFLECTION_BODY(ParticleComponent)

    public:
        ParticleComponent() {}

        void postLoadResource(std::weak_ptr<GObject> parent_object) override;

        void tick(float delta_time) override;

    private:
        void computeGlobalTransform();

        META(Enable)
        ParticleComponentRes m_particle_res;

        Math::Mat4 m_local_transform;

        ParticleEmitterTransformDesc m_transform_desc;
    };
} // namespace MarsEngine
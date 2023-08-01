#pragma once

#include "Runtime/Core/Math/Vector3.h"
#include "Runtime/Function/Render/RenderType.h"

#include <vector>

namespace MarsEngine
{
    struct PointLight
    {
        Vec3 m_position;
        // radiant flux in W
        Vec3 m_flux;

        // calculate an appropriate radius for light culling
        // a windowing function in the shader will perform a smooth transition to zero
        // this is not physically based and usually artist controlled
        float calculateRadius() const
        {
            // radius = where attenuation would lead to an intensity of 1W/m^2
            float const INTENSITY_CUTOFF = 1.0f;
            float const ATTENTUATION_CUTOFF = 0.05f;
            Vec3     intensity           = m_flux / (4.0f * Math::PI);
            float       maxIntensity        = Vec3::getMaxElement(intensity);
            float       attenuation = Math::max(INTENSITY_CUTOFF, ATTENTUATION_CUTOFF * maxIntensity) / maxIntensity;
            return 1.0f / sqrtf(attenuation);
        }
    };

    struct AmbientLight
    {
        Vec3 m_irradiance;
    };

    struct PDirectionalLight
    {
        Vec3 m_direction;
        Vec3 m_color;
    };

    struct LightList
    {
        // vertex buffers seem to be aligned to 16 bytes
        struct PointLightVertex
        {
            Vec3 m_position;
            float   m_padding;
            // radiant intensity in W/sr
            // can be calculated from radiant flux
            Vec3 m_intensity;
            float   m_radius;
        };
    };

    class PointLightList : public LightList
    {
    public:
        void init() {}
        void shutdown() {}

        // upload changes to GPU
        void update() {}

        std::vector<PointLight> m_lights;

        std::shared_ptr<BufferData> m_buffer;
    };

} // namespace MarsEngine
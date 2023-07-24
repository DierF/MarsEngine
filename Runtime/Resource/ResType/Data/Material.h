#pragma once

#include "Runtime/Core/Meta/Reflection/Reflection.h"

namespace MarsEngine
{
    REFLECTION_TYPE(MaterialRes)
    CLASS(MaterialRes, Fields)
    {
        REFLECTION_BODY(MaterialRes);

    public:
        std::string m_base_colour_texture_file;
        std::string m_metallic_roughness_texture_file;
        std::string m_normal_texture_file;
        std::string m_occlusion_texture_file;
        std::string m_emissive_texture_file;
    };
} // namespace MarsEngine
#pragma once

#include "Runtime/Core/Math/Transform.h"
#include "Runtime/Core/Meta/Reflection/Reflection.h"

namespace MarsEngine
{
    REFLECTION_TYPE(SubMeshRes)
    CLASS(SubMeshRes, Fields)
    {
        REFLECTION_BODY(SubMeshRes);

    public:
        std::string     m_obj_file_ref;
        Transform m_transform;
        std::string     m_material;
    };

    REFLECTION_TYPE(MeshComponentRes)
    CLASS(MeshComponentRes, Fields)
    {
        REFLECTION_BODY(MeshComponentRes);

    public:
        std::vector<SubMeshRes> m_sub_meshes;
    };
} // namespace MarsEngine
#pragma once

#include "Runtime/Function/Render/RenderEntity.h"
#include "Runtime/Function/Render/RenderType.h"

namespace MarsEngine
{
    class EditorTranslationAxis : public RenderEntity
    {
    public:
        EditorTranslationAxis();
        RenderMeshData m_mesh_data;
    };

    class EditorRotationAxis : public RenderEntity
    {
    public:
        EditorRotationAxis();
        RenderMeshData m_mesh_data;
    };

    class EditorScaleAxis : public RenderEntity
    {
    public:
        EditorScaleAxis();
        RenderMeshData m_mesh_data;
    };
} // namespace MarsEngine

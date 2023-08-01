#pragma once

#include "Runtime/Function/Framework/Component/Component.h"
#include "Runtime/Function/Render/RenderObject.h"
#include "Runtime/Resource/ResType/Components/Mesh.h"

#include <vector>

namespace MarsEngine
{
    class RenderSwapContext;

    REFLECTION_TYPE(MeshComponent)
    CLASS(MeshComponent : public Component, WhiteListFields)
    {
        REFLECTION_BODY(MeshComponent)
    public:
        MeshComponent() {};

        void postLoadResource(std::weak_ptr<GObject> parent_object) override;

        std::vector<GameObjectPartDesc> const& getRawMeshes() const { return m_raw_meshes; }

        void tick(float delta_time) override;

    private:
        META(Enable)
        MeshComponentRes m_mesh_res;

        std::vector<GameObjectPartDesc> m_raw_meshes;
    };
} // namespace MarsEngine

#pragma once

#include "MarsEngine/Core/Core.h"
#include "MarsEngine/Scene/Scene.h"
#include "MarsEngine/Scene/Entity.h"

namespace MarsEngine
{
	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;

		SceneHierarchyPanel(Ref<Scene> const& scene);

		void setContext(Ref<Scene> const& scene);

		void onImGuiRender();

	private:
		void drawEntityNode(Entity entity);
	
		void drawComponents(Entity entity);

	private:
		Ref<Scene> m_context;
		Entity m_selectionContext;
	};
}
#pragma once

#include "entt.hpp"
#include "MarsEngine/Core/Timestep.h"

namespace MarsEngine
{
	class Entity;

	class Scene
	{
	public:
		Scene();

		~Scene();

		Entity createEntity(std::string const& name = std::string());

		void destroyEntity(Entity entity);

		void onUpdate(Timestep ts);

		void onViewportResize(uint32_t width, uint32_t height);

	private:
		template<typename T>
		void onComponentAdded(Entity entity, T& component)
		{
		}

	private:
		entt::registry m_registry;

		uint32_t m_viewportWidth = 0, m_viewportHeight = 0;

		friend class Entity;

		friend class SceneHierarchyPanel;
	};
}
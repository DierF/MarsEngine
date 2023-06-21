#pragma once

#include "entt.hpp"
#include "MarsEngine/Core/Timestep.h"
#include "MarsEngine/Renderer/EditorCamera.h"

class b2World;

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

		void onRuntimeStart();
		void onRuntimeStop();

		void onUpdateEditor(Timestep ts, EditorCamera& camera);

		void onUpdateRuntime(Timestep ts);

		void onViewportResize(uint32_t width, uint32_t height);

		Entity getPrimaryCameraEntity();

	private:
		template<typename T>
		void onComponentAdded(Entity entity, T& component);

	private:
		entt::registry m_registry;

		uint32_t m_viewportWidth = 0, m_viewportHeight = 0;

		b2World* m_physicsWorld = nullptr;

		friend class Entity;
		friend class SceneSerializer;
		friend class SceneHierarchyPanel;
	};
}
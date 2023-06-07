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

		void onUpdate(Timestep ts);

	public:
		entt::registry m_registry;
	};
}
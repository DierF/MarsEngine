#pragma once

#include "entt.hpp"

namespace MarsEngine
{
	class Scene
	{
	public:
		Scene();

		~Scene();

	private:
		entt::registry m_registry;
	};
}
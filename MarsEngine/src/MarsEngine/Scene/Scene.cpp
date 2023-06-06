#include "pch.h"

#include "Scene.h"
#include "glm/glm.hpp"

namespace MarsEngine
{
	static void doMath(glm::mat4 const& transform)
	{

	}

	Scene::Scene()
	{
		struct TransformComponent
		{
			glm::mat4 transform;

			TransformComponent() = default;

			TransformComponent(TransformComponent const&) = default;

			TransformComponent(glm::mat4 const& transform) : transform(transform) {}

			operator glm::mat4& () { return transform; }

			operator glm::mat4 const& () const { return transform; }
		};

		entt::entity entity = m_registry.create();
		auto transform = m_registry.emplace<TransformComponent>(entity, glm::mat4(1.0f));
	}

	Scene::~Scene()
	{
	}
}
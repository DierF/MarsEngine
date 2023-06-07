#include "pch.h"

#include "MarsEngine/Renderer/Renderer2D.h"
#include "Scene.h"
#include "Components.h"
#include "Entity.h"
#include "glm/glm.hpp"

namespace MarsEngine
{
	Scene::Scene()
	{

	}

	Scene::~Scene()
	{
	}

	Entity Scene::createEntity(std::string const& name)
	{
		Entity entity = { m_registry.create(), this };
		entity.addComponent<TransformComponent>();
		auto& tag = entity.addComponent<TagComponent>();
		tag.tag = name.empty() ? "Entity" : name;
		return entity;
	}

	void Scene::onUpdate(Timestep ts)
	{
		auto group = m_registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
		for (auto entity : group)
		{
			auto& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
			
			Renderer2D::drawQuad(transform, sprite.color);
		}
	}
}
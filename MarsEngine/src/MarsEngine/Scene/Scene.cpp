#include "pch.h"

#include "MarsEngine/Renderer/Renderer2D.h"
#include "Scene.h"
#include "Component.h"
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
		m_registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
			{
				if (!nsc.instance)
				{
					nsc.instance = nsc.instantiateScript();
					nsc.instance->m_entity = Entity{ entity, this };
					nsc.instance->onCreate();
				}
				nsc.instance->onUpdate(ts);
			});

		Camera* mainCamera = nullptr;
		glm::mat4 cameraTransform;

		auto view = m_registry.view<TransformComponent, CameraComponent>();
		for (auto entity : view)
		{
			auto [ transformC, cameraC ] = view.get<TransformComponent, CameraComponent>(entity);
			if (cameraC.primary)
			{
				mainCamera = &cameraC.camera;
				cameraTransform = transformC.getTransform();
				break;
			}
		}

		if (mainCamera)
		{
			Renderer2D::beginScene(mainCamera->getProjection(), cameraTransform);

			auto group = m_registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for (auto entity : group)
			{
				auto [transformC, spriteC] = group.get<TransformComponent, SpriteRendererComponent>(entity);

				Renderer2D::drawQuad(transformC.getTransform(), spriteC.color);
			}

			Renderer2D::endScene();
		}
	}

	void Scene::onViewportResize(uint32_t width, uint32_t height)
	{
		m_viewportWidth = width;
		m_viewportHeight = height;

		auto view = m_registry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto& cameraC = view.get<CameraComponent>(entity);
			if (!cameraC.fixedAspectRatio)
			{
				cameraC.camera.setViewportSize(width, height);
			}
		}
	}
}
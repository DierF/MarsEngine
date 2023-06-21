#include "pch.h"

#include "MarsEngine/Renderer/Renderer2D.h"
#include "Scene.h"
#include "Component.h"
#include "Entity.h"
#include "glm/glm.hpp"
#include "box2d/b2_world.h"
#include "box2d/b2_body.h"
#include "box2d/b2_fixture.h"
#include "box2d/b2_polygon_shape.h"

namespace MarsEngine
{
	static b2BodyType ME_Rigidbody2DBodyTypeToBox2DBodyType(Rigidbody2DComponent::BodyType bodyType)
	{
		switch (bodyType)
		{
		case Rigidbody2DComponent::BodyType::Static:
		{
			return b2_staticBody;
		}
		case Rigidbody2DComponent::BodyType::Dynamic:
		{
			return b2_dynamicBody;
		}
		case Rigidbody2DComponent::BodyType::Kinematic:
		{
			return b2_kinematicBody;
		}
		}

		ME_CORE_ASSERT(false, "Unknown body type");
		return b2_staticBody;
	}

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

	void Scene::destroyEntity(Entity entity)
	{
		m_registry.destroy(entity);
	}

	void Scene::onRuntimeStart()
	{
		m_physicsWorld = new b2World({ 0.0f, -9.8f });
		auto view = m_registry.view<Rigidbody2DComponent>();
		for (auto e : view)
		{
			Entity entity = { e, this };
			auto& transformC = entity.getComponent<TransformComponent>();
			auto& rb2dC = entity.getComponent<Rigidbody2DComponent>();

			b2BodyDef bodyDef;
			bodyDef.type = ME_Rigidbody2DBodyTypeToBox2DBodyType(rb2dC.type);
			bodyDef.position.Set(transformC.translation.x, transformC.translation.y);
			bodyDef.angle = transformC.rotation.z;
			
			b2Body* body = m_physicsWorld->CreateBody(&bodyDef);
			body->SetFixedRotation(rb2dC.fixedRotation);
			rb2dC.runtimeBody = body;

			if (entity.hasComponent<BoxCollider2DComponent>())
			{
				auto& bc2dC = entity.getComponent<BoxCollider2DComponent>();

				b2PolygonShape boxShape;
				boxShape.SetAsBox(bc2dC.size.x * transformC.scale.x,
					bc2dC.size.y * transformC.scale.y);

				b2FixtureDef fixtureDef;
				fixtureDef.shape = &boxShape;
				fixtureDef.density = bc2dC.density;
				fixtureDef.friction = bc2dC.friction;
				fixtureDef.restitution = bc2dC.restitution;
				fixtureDef.restitutionThreshold = bc2dC.restitutionThreshold;
				body->CreateFixture(&fixtureDef);
			}
		}
	}

	void Scene::onRuntimeStop()
	{
		delete m_physicsWorld;
		m_physicsWorld = nullptr;
	}

	void Scene::onUpdateEditor(Timestep ts, EditorCamera& camera)
	{
		Renderer2D::beginScene(camera);

		auto group = m_registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
		for (auto entity : group)
		{
			auto [transformC, spriteC] = group.get<TransformComponent, SpriteRendererComponent>(entity);

			Renderer2D::drawSprite(transformC.getTransform(), spriteC, (int)entity);
		}

		Renderer2D::endScene();
	}

	void Scene::onUpdateRuntime(Timestep ts)
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

		int32_t const velocityIterations = 6;
		int32_t const positionIterations = 2;
		m_physicsWorld->Step(ts, velocityIterations, positionIterations);

		auto view = m_registry.view<Rigidbody2DComponent>();
		for (auto e : view)
		{
			Entity entity = { e, this };
			auto& transformC = entity.getComponent<TransformComponent>();
			auto& rb2dC = entity.getComponent<Rigidbody2DComponent>();

			b2Body* body = (b2Body*)rb2dC.runtimeBody;
			auto const& position = body->GetPosition();
			transformC.translation.x = position.x;
			transformC.translation.y = position.y;
			transformC.rotation.z = body->GetAngle();
		}

		Camera* mainCamera = nullptr;
		glm::mat4 cameraTransform;

		auto view2 = m_registry.view<TransformComponent, CameraComponent>();
		for (auto entity : view2)
		{
			auto [ transformC, cameraC ] = view2.get<TransformComponent, CameraComponent>(entity);
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

	Entity Scene::getPrimaryCameraEntity()
	{
		auto view = m_registry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto const& camera = view.get<CameraComponent>(entity);
			if (camera.primary)
			{
				return Entity{ entity, this };
			}
			return {};
		}
	}

	template<typename T>
	void Scene::onComponentAdded(Entity entity, T& component)
	{
	}

	template<>
	void Scene::onComponentAdded<TransformComponent>(Entity entity, TransformComponent& component)
	{
	}

	template<>
	void Scene::onComponentAdded<CameraComponent>(Entity entity, CameraComponent& component)
	{
		component.camera.setViewportSize(m_viewportWidth, m_viewportHeight);
	}

	template<>
	void Scene::onComponentAdded<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component)
	{
	}

	template<>
	void Scene::onComponentAdded<TagComponent>(Entity entity, TagComponent& component)
	{
	}

	template<>
	void Scene::onComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent& component)
	{
	}

	template<>
	void Scene::onComponentAdded<Rigidbody2DComponent>(Entity entity, Rigidbody2DComponent& component)
	{
	}

	template<>
	void Scene::onComponentAdded<BoxCollider2DComponent>(Entity entity, BoxCollider2DComponent& component)
	{
	}
}
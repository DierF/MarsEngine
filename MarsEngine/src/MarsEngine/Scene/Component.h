#pragma once

#include "SceneCamera.h"
#include "MarsEngine/Core/GUID.h"
#include "MarsEngine/Renderer/Texture.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace MarsEngine
{
	struct IDComponent
	{
		GUID id;

		IDComponent() = default;

		IDComponent(IDComponent const&) = default;
	};

	struct TagComponent
	{
		std::string tag;

		TagComponent() = default;

		TagComponent(TagComponent const&) = default;

		TagComponent(std::string const& tag) : tag(tag) {}
	};

	struct TransformComponent
	{
		glm::vec3 translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 scale = { 1.0f, 1.0f, 1.0f };

		TransformComponent() = default;

		TransformComponent(TransformComponent const&) = default;

		TransformComponent(glm::vec3 const& translation) : translation(translation) {}

		glm::mat4 getTransform() const
		{
			glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), rotation.x, { 1, 0, 0 })
				* glm::rotate(glm::mat4(1.0f), rotation.y, { 0,1,0 })
				* glm::rotate(glm::mat4(1.0f), rotation.z, { 0, 0,1 });

			return glm::translate(glm::mat4(1.0f), translation)
				* rotate
				* glm::scale(glm::mat4(1.0f), scale);
		}
	};
	
	struct SpriteRendererComponent
	{
		glm::vec4 color{ 1.0f, 1.0f, 1.0f, 1.0f };
		
		Ref<Texture2D> texture;

		float tilingFactor = 1.0f;

		SpriteRendererComponent() = default;

		SpriteRendererComponent(SpriteRendererComponent const&) = default;

		SpriteRendererComponent(glm::vec4 const& color) : color(color) {}
	};

	struct CircleRendererComponent
	{
		glm::vec4 color{ 1.0f, 1.0f, 1.0f, 1.0f };
		float thickness = 1.0f;
		float fade = 0.005f;

		CircleRendererComponent() = default;
		CircleRendererComponent(CircleRendererComponent const&) = default;
	};

	struct CameraComponent
	{
		SceneCamera camera;

		bool primary = true;

		bool fixedAspectRatio = false;

		CameraComponent() = default;

		CameraComponent(CameraComponent const&) = default;
	};

	class ScriptableEntity;

	struct NativeScriptComponent
	{
		ScriptableEntity* instance = nullptr;

		ScriptableEntity*(*instantiateScript)();

		void(*destroyScript)(NativeScriptComponent*);

		template<typename T>
		void bind()
		{
			instantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); };

			destroyScript = [](NativeScriptComponent* nsc) { delete nsc->instance; nsc->instance = nullptr; };
		}
	};

	struct Rigidbody2DComponent
	{
		enum class BodyType
		{
			Static = 0, Dynamic, Kinematic
		} type = BodyType::Static;

		bool fixedRotation = false;

		void* runtimeBody = nullptr;

		Rigidbody2DComponent() = default;

		Rigidbody2DComponent(Rigidbody2DComponent const&) = default;
	};

	struct BoxCollider2DComponent
	{
		glm::vec2 offset = { 0.0f, 0.0f };
		glm::vec2 size = { 0.5f, 0.5f };

		float density = 1.0f;
		float friction = 0.5f;
		float restitution = 0.0f;
		float restitutionThreshold = 0.5f;

		void* runtimeFixture = nullptr;

		BoxCollider2DComponent() = default;

		BoxCollider2DComponent(BoxCollider2DComponent const&) = default;
	};
}
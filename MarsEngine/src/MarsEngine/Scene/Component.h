#pragma once

#include "glm/glm.hpp"
#include "SceneCamera.h"
#include "ScriptableEntity.h"

namespace MarsEngine
{
	struct TagComponent
	{
		std::string tag;

		TagComponent() = default;

		TagComponent(TagComponent const&) = default;

		TagComponent(std::string const& tag) : tag(tag) {}
	};

	struct TransformComponent
	{
		glm::mat4 transform = glm::mat4(1.0f);

		TransformComponent() = default;

		TransformComponent(TransformComponent const&) = default;

		TransformComponent(glm::mat4 const& transform) : transform(transform) {}

		operator glm::mat4& () { return transform; }

		operator glm::mat4 const& () const { return transform; }
	};
	
	struct SpriteRendererComponent
	{
		glm::vec4 color{ 1.0f, 1.0f, 1.0f, 1.0f };

		SpriteRendererComponent() = default;

		SpriteRendererComponent(SpriteRendererComponent const&) = default;

		SpriteRendererComponent(glm::vec4 const& color) : color(color) {}
	};

	struct CameraComponent
	{
		SceneCamera camera;

		bool primary = true;

		bool fixedAspectRatio = false;

		CameraComponent() = default;

		CameraComponent(CameraComponent const&) = default;
	};

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
}
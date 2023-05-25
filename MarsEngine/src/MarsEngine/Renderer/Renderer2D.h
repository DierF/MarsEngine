#pragma once

#include "OrthographicCamera.h"

#include "Texture.h"

namespace MarsEngine
{
	class Renderer2D
	{
	public:
		static void init();

		static void shutdown();

		static void beginScene(OrthographicCamera const& camera);

		static void endScene();

		static void drawQuad(glm::vec2 const& position, glm::vec2 const& size, glm::vec4 const& color);

		static void drawQuad(glm::vec3 const& position, glm::vec2 const& size, glm::vec4 const& color);
		
		static void drawQuad(glm::vec2 const& position, glm::vec2 const& size, Ref<Texture2D> const& texture);

		static void drawQuad(glm::vec3 const& position, glm::vec2 const& size, Ref<Texture2D> const& texture);
	};
}
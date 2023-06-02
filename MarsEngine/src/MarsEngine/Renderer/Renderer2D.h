#pragma once

#include "OrthographicCamera.h"
#include "Texture.h"
#include "SubTexture2D.h"

namespace MarsEngine
{
	class Renderer2D
	{
	public:
		static void init();

		static void shutdown();

		static void beginScene(OrthographicCamera const& camera);

		static void endScene();

		static void flush();

		static void drawQuad(glm::vec2 const& position, glm::vec2 const& size, glm::vec4 const& color);

		static void drawQuad(glm::vec3 const& position, glm::vec2 const& size, glm::vec4 const& color);
		
		static void drawQuad(glm::vec2 const& position, glm::vec2 const& size, Ref<Texture2D> const& texture, float tilingFactor = 1.0f, glm::vec4 const& tintColor = glm::vec4(1.0f));

		static void drawQuad(glm::vec3 const& position, glm::vec2 const& size, Ref<Texture2D> const& texture, float tilingFactor = 1.0f, glm::vec4 const& tintColor = glm::vec4(1.0f));
		
		static void drawQuad(glm::vec2 const& position, glm::vec2 const& size, Ref<SubTexture2D> const& subTexture, float tilingFactor = 1.0f, glm::vec4 const& tintColor = glm::vec4(1.0f));

		static void drawQuad(glm::vec3 const& position, glm::vec2 const& size, Ref<SubTexture2D> const& subTexture, float tilingFactor = 1.0f, glm::vec4 const& tintColor = glm::vec4(1.0f));
	
		static void drawRotatedQuad(glm::vec2 const& position, glm::vec2 const& size, float rotation, glm::vec4 const& color);
		
		static void drawRotatedQuad(glm::vec3 const& position, glm::vec2 const& size, float rotation, glm::vec4 const& color);
		
		static void drawRotatedQuad(glm::vec2 const& position, glm::vec2 const& size, float rotation, Ref<Texture2D> const& texture, float tilingFactor = 1.0f, glm::vec4 const& tintColor = glm::vec4(1.0f));
		
		static void drawRotatedQuad(glm::vec3 const& position, glm::vec2 const& size, float rotation, Ref<Texture2D> const& texture, float tilingFactor = 1.0f, glm::vec4 const& tintColor = glm::vec4(1.0f));
		
		static void drawRotatedQuad(glm::vec2 const& position, glm::vec2 const& size, float rotation, Ref<SubTexture2D> const& subTexture, float tilingFactor = 1.0f, glm::vec4 const& tintColor = glm::vec4(1.0f));
		
		static void drawRotatedQuad(glm::vec3 const& position, glm::vec2 const& size, float rotation, Ref<SubTexture2D> const& subTexture, float tilingFactor = 1.0f, glm::vec4 const& tintColor = glm::vec4(1.0f));
	
		struct Statistics
		{
			uint32_t drawCalls = 0;
			uint32_t quadCount = 0;

			uint32_t getTotalVertexCount() { return quadCount * 4; }
			uint32_t getTotalIndexCount() { return quadCount * 6; }
		};

		static void resetStats();

		static Statistics getStats();

	private:
		static void flushAndReset();
	};
}
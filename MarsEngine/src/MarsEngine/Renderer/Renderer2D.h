#pragma once

#include "OrthographicCamera.h"
#include "Texture.h"
#include "SubTexture2D.h"
#include "Camera.h"
#include "EditorCamera.h"
#include "MarsEngine/Scene/Component.h"

namespace MarsEngine
{
	class Renderer2D
	{
	public:
		static void init();

		static void shutdown();

		static void beginScene(Camera const& camera, glm::mat4 const& transform);

		static void beginScene(EditorCamera const& camera);

		static void beginScene(OrthographicCamera const& camera);

		static void endScene();

		static void flush();

		static void drawQuad(glm::vec2 const& position, glm::vec2 const& size, glm::vec4 const& color);
		static void drawQuad(glm::vec3 const& position, glm::vec2 const& size, glm::vec4 const& color);
		static void drawQuad(glm::vec2 const& position, glm::vec2 const& size, Ref<Texture2D> const& texture, float tilingFactor = 1.0f, glm::vec4 const& tintColor = glm::vec4(1.0f));
		static void drawQuad(glm::vec3 const& position, glm::vec2 const& size, Ref<Texture2D> const& texture, float tilingFactor = 1.0f, glm::vec4 const& tintColor = glm::vec4(1.0f));
		static void drawQuad(glm::vec2 const& position, glm::vec2 const& size, Ref<SubTexture2D> const& subTexture, float tilingFactor = 1.0f, glm::vec4 const& tintColor = glm::vec4(1.0f));
		static void drawQuad(glm::vec3 const& position, glm::vec2 const& size, Ref<SubTexture2D> const& subTexture, float tilingFactor = 1.0f, glm::vec4 const& tintColor = glm::vec4(1.0f));
		
		static void drawQuad(glm::mat4 const& transform, glm::vec4 const& color, int entityID = -1);
		static void drawQuad(glm::mat4 const& transform, Ref<Texture2D> const& texture, float tilingFactor = 1.0f, glm::vec4 const& tintColor = glm::vec4(1.0f), int entityID = -1);

		static void drawRotatedQuad(glm::vec2 const& position, glm::vec2 const& size, float rotation, glm::vec4 const& color);
		static void drawRotatedQuad(glm::vec3 const& position, glm::vec2 const& size, float rotation, glm::vec4 const& color);
		static void drawRotatedQuad(glm::vec2 const& position, glm::vec2 const& size, float rotation, Ref<Texture2D> const& texture, float tilingFactor = 1.0f, glm::vec4 const& tintColor = glm::vec4(1.0f));
		static void drawRotatedQuad(glm::vec3 const& position, glm::vec2 const& size, float rotation, Ref<Texture2D> const& texture, float tilingFactor = 1.0f, glm::vec4 const& tintColor = glm::vec4(1.0f));
		static void drawRotatedQuad(glm::vec2 const& position, glm::vec2 const& size, float rotation, Ref<SubTexture2D> const& subTexture, float tilingFactor = 1.0f, glm::vec4 const& tintColor = glm::vec4(1.0f));
		static void drawRotatedQuad(glm::vec3 const& position, glm::vec2 const& size, float rotation, Ref<SubTexture2D> const& subTexture, float tilingFactor = 1.0f, glm::vec4 const& tintColor = glm::vec4(1.0f));
	
		static void drawCircle(glm::mat4 const& transform, glm::vec4 const& color, float thickness = 1.0f, float fade = 0.005f, int entityID = -1);
		
		static void drawLine(glm::vec3 const& position0, glm::vec3 const& position1, glm::vec4 const& color, int entityID = -1);
		
		static void drawRect(glm::vec3 const& position, glm::vec2 const& size, glm::vec4 const& color, int entityID = -1);
		static void drawRect(glm::mat4 const& transform, glm::vec4 const& color, int entityID = -1);

		static void drawSprite(glm::mat4 const& transform, SpriteRendererComponent& spriteC, int entityID);
		
		static float getLineWidth();
		static void setLineWidth(float width);
		
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

		static void startBatch();
	};
}
#include "pch.h"

#include "Renderer2D.h"
#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"
#include "glm/gtc/matrix_transform.hpp"

namespace MarsEngine
{
	struct QuadVertex
	{
		glm::vec3 position;
		glm::vec4 color;
		glm::vec2 textCoord;
		float textureIndex;
		float tilingFactor;

		//Editor-only
		int entityID;
	};

	struct Renderer2DData
	{
		static uint32_t const maxQuad = 20'000;
		static uint32_t const maxVertices = maxQuad * 4;
		static uint32_t const maxIndices = maxQuad * 6;
		static uint32_t const maxTextureSlots = 32;

		Ref<VertexArray> quadVertexArray;
		Ref<VertexBuffer> quadVertexBuffer;
		Ref<Shader> textureShader;
		Ref<Texture2D>whiteTexture;

		uint32_t quadIndexCount = 0;
		QuadVertex* quadVertexBufferBase = nullptr;
		QuadVertex* quadVertexBufferPtr = nullptr;

		std::array<Ref<Texture2D>, maxTextureSlots> textureSlots;
		uint32_t textureSlotIndex = 1;

		glm::vec4 quadVertexPositions[4];

		Renderer2D::Statistics stats;
	};

	static Renderer2DData s_data;

	void Renderer2D::init()
	{
		ME_PROFILE_FUNCTION();

		s_data.quadVertexArray = VertexArray::create();

		s_data.quadVertexBuffer = VertexBuffer::create(s_data.maxVertices * sizeof(QuadVertex));

		BufferLayout quadLayout =
		{
			{ ShaderDataType::Float3, "a_position" },
			{ ShaderDataType::Float4, "a_color" },
			{ ShaderDataType::Float2, "a_textCoord" },
			{ ShaderDataType::Float, "a_textIndex" },
			{ ShaderDataType::Float, "a_tilingFactor" },
			{ ShaderDataType::Int, "a_entityID" }
		};

		s_data.quadVertexBuffer->setLayout(quadLayout);
		s_data.quadVertexArray->addVertexBuffer(s_data.quadVertexBuffer);

		s_data.quadVertexBufferBase = new QuadVertex[s_data.maxVertices];

		uint32_t* quadIndices = new uint32_t[s_data.maxIndices];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_data.maxIndices; i += 6)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}

		Ref<IndexBuffer> quadIB = IndexBuffer::create(quadIndices, s_data.maxIndices);
		s_data.quadVertexArray->setIndexBuffer(quadIB);
		delete[] quadIndices;

		s_data.whiteTexture = Texture2D::create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_data.whiteTexture->setData(&whiteTextureData, sizeof(uint32_t));

		int32_t samplers[s_data.maxTextureSlots];
		for (uint32_t i = 0; i < s_data.maxTextureSlots; ++i)
		{
			samplers[i] = i;
		}

		s_data.textureShader = Shader::create("assets/shaders/Texture.glsl");
		s_data.textureShader->bind();
		s_data.textureShader->setIntArray("u_texture", samplers, s_data.maxTextureSlots);

		s_data.textureSlots[0] = s_data.whiteTexture;

		s_data.quadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		s_data.quadVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
		s_data.quadVertexPositions[2] = { 0.5f, 0.5f, 0.0f, 1.0f };
		s_data.quadVertexPositions[3] = { -0.5f, 0.5f, 0.0f, 1.0f };
	}

	void Renderer2D::shutdown()
	{
		ME_PROFILE_FUNCTION();
	}

	void Renderer2D::beginScene(Camera const& camera, glm::mat4 const& transform)
	{
		glm::mat4 viewProjection = camera.getProjection() * glm::inverse(transform);

		s_data.textureShader->bind();
		s_data.textureShader->setMat4("u_viewProjection", viewProjection);

		startBatch();
	}

	void Renderer2D::beginScene(EditorCamera const& camera)
	{
		glm::mat4 viewProjection = camera.getViewProjection();

		s_data.textureShader->bind();
		s_data.textureShader->setMat4("u_viewProjection", viewProjection);

		startBatch();
	}

	void Renderer2D::beginScene(OrthographicCamera const& camera)
	{
		s_data.textureShader->bind();
		s_data.textureShader->setMat4("u_viewProjection", camera.getViewProjectionMatrix());

		startBatch();
	}

	void Renderer2D::endScene()
	{
		ME_PROFILE_FUNCTION();

		uint32_t dataSize = (uint32_t)((uint8_t*)s_data.quadVertexBufferPtr - (uint8_t*)s_data.quadVertexBufferBase);
		if (dataSize)
		{
			s_data.quadVertexBuffer->setData(s_data.quadVertexBufferBase, dataSize);
			flush();
		}

	}

	void Renderer2D::flush()
	{
		for (uint32_t i = 0; i < s_data.textureSlotIndex; ++i)
		{
			s_data.textureSlots[i]->bind(i);
		}

		RenderCommand::drawIndexed(s_data.quadVertexArray, s_data.quadIndexCount);
		++s_data.stats.drawCalls;
	}

	void Renderer2D::flushAndReset()
	{
		endScene();

		startBatch();
	}

	void Renderer2D::startBatch()
	{
		s_data.quadIndexCount = 0;
		s_data.quadVertexBufferPtr = s_data.quadVertexBufferBase;

		s_data.textureSlotIndex = 1;
	}

	void Renderer2D::drawQuad(glm::vec2 const& position, glm::vec2 const& size, glm::vec4 const& color)
	{
		drawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::drawQuad(glm::vec3 const& position, glm::vec2 const& size, glm::vec4 const& color)
	{
		ME_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		drawQuad(transform, color);
	}

	void Renderer2D::drawQuad(glm::vec2 const& position, glm::vec2 const& size, Ref<Texture2D> const& texture, float tilingFactor, glm::vec4 const& tintColor)
	{
		drawQuad({ position.x, position.y, 0.0f }, size, texture, tilingFactor, tintColor);
	}

	void Renderer2D::drawQuad(glm::vec3 const& position, glm::vec2 const& size, Ref<Texture2D> const& texture, float tilingFactor, glm::vec4 const& tintColor)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		drawQuad(transform, texture, tilingFactor);
	}

	void Renderer2D::drawQuad(glm::vec2 const& position, glm::vec2 const& size, Ref<SubTexture2D> const& subTexture, float tilingFactor, glm::vec4 const& tintColor)
	{
		drawQuad({ position.x, position.y, 0.0f }, size, subTexture, tilingFactor, tintColor);
	}

	void Renderer2D::drawQuad(glm::vec3 const& position, glm::vec2 const& size, Ref<SubTexture2D> const& subTexture, float tilingFactor, glm::vec4 const& tintColor)
	{
		ME_PROFILE_FUNCTION();

		if (s_data.quadIndexCount >= Renderer2DData::maxIndices)
		{
			flushAndReset();
		}

		glm::vec4 const color = { 1.0f, 1.0f, 1.0f, 1.0f };

		auto const textureCoords = subTexture->getTextureCoords();
		auto const texture = subTexture->getTexture();

		float textureIndex = 0.0f;


		for (uint32_t i = 1; i < s_data.textureSlotIndex; ++i)
		{
			if (*s_data.textureSlots[i].get() == *texture.get())
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			textureIndex = (float)s_data.textureSlotIndex;
			s_data.textureSlots[s_data.textureSlotIndex] = texture;
			++s_data.textureSlotIndex;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		constexpr glm::vec2 textureCoordss[] =
		{ { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		for (size_t i = 0; i < 4; ++i)
		{
			s_data.quadVertexBufferPtr->position = transform * s_data.quadVertexPositions[i];
			s_data.quadVertexBufferPtr->color = color;
			s_data.quadVertexBufferPtr->textCoord = textureCoordss[i];
			s_data.quadVertexBufferPtr->textureIndex = textureIndex;
			s_data.quadVertexBufferPtr->tilingFactor = tilingFactor;
			++s_data.quadVertexBufferPtr;
		}

		s_data.quadIndexCount += 6;

		++s_data.stats.quadCount;
	}

	void Renderer2D::drawQuad(glm::mat4 const& transform, glm::vec4 const& color, int entityID)
	{
		if (s_data.quadIndexCount >= Renderer2DData::maxIndices)
		{
			flushAndReset();
		}

		float const textureIndex = 0.0f;

		float const tilingFactor = 1.0f;

		constexpr glm::vec2 textureCoordss[] =
		{ { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		for (size_t i = 0; i < 4; ++i)
		{
			s_data.quadVertexBufferPtr->position = transform * s_data.quadVertexPositions[i];
			s_data.quadVertexBufferPtr->color = color;
			s_data.quadVertexBufferPtr->textCoord = textureCoordss[i];
			s_data.quadVertexBufferPtr->textureIndex = textureIndex;
			s_data.quadVertexBufferPtr->tilingFactor = tilingFactor;
			s_data.quadVertexBufferPtr->entityID = entityID;
			++s_data.quadVertexBufferPtr;
		}

		s_data.quadIndexCount += 6;

		++s_data.stats.quadCount;
	}

	void Renderer2D::drawQuad(glm::mat4 const& transform, Ref<Texture2D> const& texture, float tilingFactor, glm::vec4 const& tintColor, int entityID)
	{
		if (s_data.quadIndexCount >= Renderer2DData::maxIndices)
		{
			flushAndReset();
		}

		glm::vec4 const color = { 1.0f, 1.0f, 1.0f, 1.0f };

		float textureIndex = 0.0f;

		for (uint32_t i = 1; i < s_data.textureSlotIndex; ++i)
		{
			if (*s_data.textureSlots[i].get() == *texture.get())
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			textureIndex = (float)s_data.textureSlotIndex;
			s_data.textureSlots[s_data.textureSlotIndex] = texture;
			++s_data.textureSlotIndex;
		}

		constexpr glm::vec2 textureCoordss[] =
		{ { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		for (size_t i = 0; i < 4; ++i)
		{
			s_data.quadVertexBufferPtr->position = transform * s_data.quadVertexPositions[i];
			s_data.quadVertexBufferPtr->color = color;
			s_data.quadVertexBufferPtr->textCoord = textureCoordss[i];
			s_data.quadVertexBufferPtr->textureIndex = textureIndex;
			s_data.quadVertexBufferPtr->tilingFactor = tilingFactor;
			s_data.quadVertexBufferPtr->entityID = entityID;
			++s_data.quadVertexBufferPtr;
		}

		s_data.quadIndexCount += 6;

		++s_data.stats.quadCount;
	}


	void Renderer2D::drawRotatedQuad(glm::vec2 const& position, glm::vec2 const& size, float rotation, glm::vec4 const& color)
	{
		drawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, color);
	}

	void Renderer2D::drawRotatedQuad(glm::vec3 const& position, glm::vec2 const& size, float rotation, glm::vec4 const& color)
	{
		ME_PROFILE_FUNCTION();

		if (s_data.quadIndexCount >= Renderer2DData::maxIndices)
		{
			flushAndReset();
		}

		float const textureIndex = 0.0f;

		float const tilingFactor = 1.0f;

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		constexpr glm::vec2 textureCoordss[] =
		{ { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		for (size_t i = 0; i < 4; ++i)
		{
			s_data.quadVertexBufferPtr->position = transform * s_data.quadVertexPositions[i];
			s_data.quadVertexBufferPtr->color = color;
			s_data.quadVertexBufferPtr->textCoord = textureCoordss[i];
			s_data.quadVertexBufferPtr->textureIndex = textureIndex;
			s_data.quadVertexBufferPtr->tilingFactor = tilingFactor;
			++s_data.quadVertexBufferPtr;
		}

		s_data.quadIndexCount += 6;

		++s_data.stats.quadCount;
	}

	void Renderer2D::drawRotatedQuad(glm::vec2 const& position, glm::vec2 const& size, float rotation, Ref<Texture2D> const& texture, float tilingFactor, glm::vec4 const& tintColor)
	{
		drawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, texture, tilingFactor, tintColor);
	}

	void Renderer2D::drawRotatedQuad(glm::vec3 const& position, glm::vec2 const& size, float rotation, Ref<Texture2D> const& texture, float tilingFactor, glm::vec4 const& tintColor)
	{
		ME_PROFILE_FUNCTION();

		if (s_data.quadIndexCount >= Renderer2DData::maxIndices)
		{
			flushAndReset();
		}

		glm::vec4 const color = { 1.0f, 1.0f, 1.0f, 1.0f };

		float textureIndex = 0.0f;

		for (uint32_t i = 1; i < s_data.textureSlotIndex; ++i)
		{
			if (*s_data.textureSlots[i].get() == *texture.get())
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			textureIndex = (float)s_data.textureSlotIndex;
			s_data.textureSlots[s_data.textureSlotIndex] = texture;
			++s_data.textureSlotIndex;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		constexpr glm::vec2 textureCoordss[] =
		{ { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		for (size_t i = 0; i < 4; ++i)
		{
			s_data.quadVertexBufferPtr->position = transform * s_data.quadVertexPositions[i];
			s_data.quadVertexBufferPtr->color = color;
			s_data.quadVertexBufferPtr->textCoord = textureCoordss[i];
			s_data.quadVertexBufferPtr->textureIndex = textureIndex;
			s_data.quadVertexBufferPtr->tilingFactor = tilingFactor;
			++s_data.quadVertexBufferPtr;
		}

		s_data.quadIndexCount += 6;

		++s_data.stats.quadCount;
	}

	void Renderer2D::drawRotatedQuad(glm::vec2 const& position, glm::vec2 const& size, float rotation, Ref<SubTexture2D> const& subTexture, float tilingFactor, glm::vec4 const& tintColor)
	{
		drawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, subTexture, tilingFactor, tintColor);
	}

	void Renderer2D::drawRotatedQuad(glm::vec3 const& position, glm::vec2 const& size, float rotation, Ref<SubTexture2D> const& subTexture, float tilingFactor, glm::vec4 const& tintColor)
	{
		ME_PROFILE_FUNCTION();

		if (s_data.quadIndexCount >= Renderer2DData::maxIndices)
		{
			flushAndReset();
		}

		auto const textureCoords = subTexture->getTextureCoords();
		auto const texture = subTexture->getTexture();
		glm::vec4 const color = { 1.0f, 1.0f, 1.0f, 1.0f };

		float textureIndex = 0.0f;

		for (uint32_t i = 1; i < s_data.textureSlotIndex; ++i)
		{
			if (*s_data.textureSlots[i].get() == *texture.get())
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			textureIndex = (float)s_data.textureSlotIndex;
			s_data.textureSlots[s_data.textureSlotIndex] = texture;
			++s_data.textureSlotIndex;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		constexpr glm::vec2 textureCoordss[] =
		{ { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		for (size_t i = 0; i < 4; ++i)
		{
			s_data.quadVertexBufferPtr->position = transform * s_data.quadVertexPositions[i];
			s_data.quadVertexBufferPtr->color = color;
			s_data.quadVertexBufferPtr->textCoord = textureCoordss[i];
			s_data.quadVertexBufferPtr->textureIndex = textureIndex;
			s_data.quadVertexBufferPtr->tilingFactor = tilingFactor;
			++s_data.quadVertexBufferPtr;
		}

		s_data.quadIndexCount += 6;

		++s_data.stats.quadCount;
	}

	void Renderer2D::drawSprite(glm::mat4 const& transform, SpriteRendererComponent& spriteC, int entityID)
	{
		if (spriteC.texture)
		{
			drawQuad(transform, spriteC.texture, spriteC.tilingFactor, spriteC.color, entityID);
		}
		else
		{
			drawQuad(transform, spriteC.color, entityID);
		}
	}

	void Renderer2D::resetStats()
	{
		memset(&s_data.stats, 0, sizeof(Statistics));
	}

	Renderer2D::Statistics Renderer2D::getStats()
	{
		return s_data.stats;
	}
}
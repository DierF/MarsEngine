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

	struct CircleVertex
	{
		glm::vec3 worldPosition;
		glm::vec3 localPosition;
		glm::vec4 color;
		float thickness;
		float fade;

		// Editor-only
		int entityID;
	};

	struct LineVertex
	{
		glm::vec3 position;
		glm::vec4 color;

		// Editor-only
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
		Ref<Shader> quadShader;
		Ref<Texture2D>whiteTexture;

		Ref<VertexArray> circleVertexArray;
		Ref<VertexBuffer> circleVertexBuffer;
		Ref<Shader> circleShader;

		Ref<VertexArray> lineVertexArray;
		Ref<VertexBuffer> lineVertexBuffer;
		Ref<Shader> lineShader;

		uint32_t quadIndexCount = 0;
		QuadVertex* quadVertexBufferBase = nullptr;
		QuadVertex* quadVertexBufferPtr = nullptr;

		uint32_t circleIndexCount = 0;
		CircleVertex* circleVertexBufferBase = nullptr;
		CircleVertex* circleVertexBufferPtr = nullptr;

		uint32_t lineVertexCount = 0;
		LineVertex* lineVertexBufferBase = nullptr;
		LineVertex* lineVertexBufferPtr = nullptr;
		float lineWidth = 2.0f;

		std::array<Ref<Texture2D>, maxTextureSlots> textureSlots;
		uint32_t textureSlotIndex = 1;

		glm::vec4 quadVertexPositions[4];

		Renderer2D::Statistics stats;
	};

	static Renderer2DData s_data;

	void Renderer2D::init()
	{
		//quad
		s_data.quadVertexArray = VertexArray::create();

		s_data.quadVertexBuffer = VertexBuffer::create(s_data.maxVertices
			* sizeof(QuadVertex));

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

		//circle
		s_data.circleVertexArray = VertexArray::create();

		s_data.circleVertexBuffer = VertexBuffer::create(s_data.maxVertices
			* sizeof(CircleVertex));

		BufferLayout circleLayout =
		{
			{ ShaderDataType::Float3, "a_worldPosition" },
			{ ShaderDataType::Float3, "a_localPosition" },
			{ ShaderDataType::Float4, "a_color" },
			{ ShaderDataType::Float, "a_thickness" },
			{ ShaderDataType::Float, "a_fade" },
			{ ShaderDataType::Int, "a_entityID" }
		};
		s_data.circleVertexBuffer->setLayout(circleLayout);

		s_data.circleVertexArray->addVertexBuffer(s_data.circleVertexBuffer);
		s_data.circleVertexArray->setIndexBuffer(quadIB); // use quadIB
		s_data.circleVertexBufferBase = new CircleVertex[s_data.maxVertices];

		//line
		s_data.lineVertexArray = VertexArray::create();

		s_data.lineVertexBuffer = VertexBuffer::create(s_data.maxVertices
			* sizeof(LineVertex));

		BufferLayout lineLayout =
		{
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_color" },
			{ ShaderDataType::Int, "a_entityID" }
		};
		s_data.lineVertexBuffer->setLayout(lineLayout);

		s_data.lineVertexArray->addVertexBuffer(s_data.lineVertexBuffer);
		s_data.lineVertexBufferBase = new LineVertex[s_data.maxVertices];


		s_data.whiteTexture = Texture2D::create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_data.whiteTexture->setData(&whiteTextureData, sizeof(uint32_t));


		int32_t samplers[s_data.maxTextureSlots];
		for (uint32_t i = 0; i < s_data.maxTextureSlots; ++i)
		{
			samplers[i] = i;
		}

		s_data.quadShader = Shader::create("assets/shaders/QuadTexture.glsl");
		s_data.circleShader = Shader::create("assets/shaders/CircleTexture.glsl");
		s_data.lineShader = Shader::create("assets/shaders/LineTexture.glsl");
		
		s_data.quadShader->setIntArray("u_texture", samplers, s_data.maxTextureSlots);

		s_data.textureSlots[0] = s_data.whiteTexture;

		s_data.quadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		s_data.quadVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
		s_data.quadVertexPositions[2] = { 0.5f, 0.5f, 0.0f, 1.0f };
		s_data.quadVertexPositions[3] = { -0.5f, 0.5f, 0.0f, 1.0f };
	}

	void Renderer2D::shutdown()
	{
		delete[] s_data.quadVertexBufferBase;
	}

	void Renderer2D::beginScene(Camera const& camera, glm::mat4 const& transform)
	{
		glm::mat4 viewProjection = camera.getProjection() * glm::inverse(transform);

		s_data.quadShader->bind();
		s_data.quadShader->setMat4("u_viewProjection", viewProjection);

		s_data.circleShader->bind();
		s_data.circleShader->setMat4("u_viewProjection", viewProjection);
		
		s_data.lineShader->bind();
		s_data.lineShader->setMat4("u_viewProjection", viewProjection);

		startBatch();
	}

	void Renderer2D::beginScene(EditorCamera const& camera)
	{
		glm::mat4 viewProjection = camera.getViewProjection();

		s_data.quadShader->bind();
		s_data.quadShader->setMat4("u_viewProjection", viewProjection);

		s_data.circleShader->bind();
		s_data.circleShader->setMat4("u_viewProjection", viewProjection);

		s_data.lineShader->bind();
		s_data.lineShader->setMat4("u_viewProjection", viewProjection);

		startBatch();
	}

	void Renderer2D::beginScene(OrthographicCamera const& camera)
	{
		s_data.quadShader->bind();
		s_data.quadShader->setMat4("u_viewProjection", camera.getViewProjectionMatrix());
		
		s_data.circleShader->bind();
		s_data.circleShader->setMat4("u_viewProjection", camera.getViewProjectionMatrix());

		s_data.lineShader->bind();
		s_data.lineShader->setMat4("u_viewProjection", camera.getViewProjectionMatrix());

		startBatch();
	}

	void Renderer2D::endScene()
	{
		flush();
	}

	void Renderer2D::flush()
	{
		if (s_data.quadIndexCount)
		{
			uint32_t dataSize = (uint32_t)((uint8_t*)s_data.quadVertexBufferPtr
				- (uint8_t*)s_data.quadVertexBufferBase);
			s_data.quadVertexBuffer->setData(s_data.quadVertexBufferBase, dataSize);

			for (uint32_t i = 0; i < s_data.textureSlotIndex; ++i)
			{
				s_data.textureSlots[i]->bind(i);
			}
			s_data.quadShader->bind();
			RenderCommand::drawIndexed(s_data.quadVertexArray, s_data.quadIndexCount);
			++s_data.stats.drawCalls;
		}
		if (s_data.circleIndexCount)
		{
			uint32_t dataSize = (uint32_t)((uint8_t*)s_data.circleVertexBufferPtr
				- (uint8_t*)s_data.circleVertexBufferBase);
			s_data.circleVertexBuffer->setData(s_data.circleVertexBufferBase, dataSize);

			s_data.circleShader->bind();
			RenderCommand::drawIndexed(s_data.circleVertexArray, s_data.circleIndexCount);
			++s_data.stats.drawCalls;
		}
		if (s_data.lineVertexCount)
		{
			uint32_t dataSize = (uint32_t)((uint8_t*)s_data.lineVertexBufferPtr
				- (uint8_t*)s_data.lineVertexBufferBase);
			s_data.lineVertexBuffer->setData(s_data.lineVertexBufferBase, dataSize);

			s_data.lineShader->bind();
			RenderCommand::setLineWidth(s_data.lineWidth);
			RenderCommand::drawLines(s_data.lineVertexArray, s_data.lineVertexCount);
			++s_data.stats.drawCalls;
		}
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

		s_data.circleIndexCount = 0;
		s_data.circleVertexBufferPtr = s_data.circleVertexBufferBase;
		
		s_data.lineVertexCount = 0;
		s_data.lineVertexBufferPtr = s_data.lineVertexBufferBase;

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

	void Renderer2D::drawCircle(glm::mat4 const& transform, glm::vec4 const& color, float thickness, float fade, int entityID)
	{
		//TODO: implement for circle
		//if (s_data.quadIndexCount >= Renderer2DData::maxIndices)
		//{
		//	flushAndReset();
		//}

		for (size_t i = 0; i < 4; ++i)
		{
			s_data.circleVertexBufferPtr->worldPosition = transform * s_data.quadVertexPositions[i];
			s_data.circleVertexBufferPtr->localPosition = s_data.quadVertexPositions[i] * 2.0f;
			s_data.circleVertexBufferPtr->color = color;
			s_data.circleVertexBufferPtr->thickness = thickness;
			s_data.circleVertexBufferPtr->fade = fade;
			s_data.circleVertexBufferPtr->entityID = entityID;
			++s_data.circleVertexBufferPtr;
		}

		s_data.circleIndexCount += 6;

		++s_data.stats.quadCount;
	}

	void Renderer2D::drawLine(glm::vec3 const& position0, glm::vec3 const& position1, glm::vec4 const& color, int entityID)
	{
		s_data.lineVertexBufferPtr->position = position0;
		s_data.lineVertexBufferPtr->color = color;
		s_data.lineVertexBufferPtr->entityID = entityID;
		++s_data.lineVertexBufferPtr;
		
		s_data.lineVertexBufferPtr->position = position1;
		s_data.lineVertexBufferPtr->color = color;
		s_data.lineVertexBufferPtr->entityID = entityID;
		++s_data.lineVertexBufferPtr;

		s_data.lineVertexCount += 2;
	}

	void Renderer2D::drawRect(glm::vec3 const& position, glm::vec2 const& size, glm::vec4 const& color, int entityID)
	{
		glm::vec3 p0 = glm::vec3(position.x - size.x * 0.5f, position.y - size.y * 0.5f, position.z);
		glm::vec3 p1 = glm::vec3(position.x + size.x * 0.5f, position.y - size.y * 0.5f, position.z);
		glm::vec3 p2 = glm::vec3(position.x + size.x * 0.5f, position.y + size.y * 0.5f, position.z);
		glm::vec3 p3 = glm::vec3(position.x - size.x * 0.5f, position.y + size.y * 0.5f, position.z);

		drawLine(p0, p1, color);
		drawLine(p1, p2, color);
		drawLine(p2, p3, color);
		drawLine(p3, p0, color);
	}

	void Renderer2D::drawRect(glm::mat4 const& transform, glm::vec4 const& color, int entityID)
	{
		glm::vec3 lineVertices[4];
		for (size_t i = 0; i < 4; ++i)
		{
			lineVertices[i] = transform * s_data.quadVertexPositions[i];
		}

		drawLine(lineVertices[0], lineVertices[1], color);
		drawLine(lineVertices[1], lineVertices[2], color);
		drawLine(lineVertices[2], lineVertices[3], color);
		drawLine(lineVertices[3], lineVertices[0], color);
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

	float Renderer2D::getLineWidth()
	{
		return s_data.lineWidth;
	}

	void Renderer2D::setLineWidth(float width)
	{
		s_data.lineWidth = width;
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
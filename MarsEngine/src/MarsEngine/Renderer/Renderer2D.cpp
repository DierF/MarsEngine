#include "pch.h"

#include "Renderer2D.h"
#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"
#include "glm/gtc/matrix_transform.hpp"

namespace MarsEngine
{
	struct Renderer2DStorage
	{
		Ref<VertexArray> quadVertexArray;
		Ref<Shader> textureShader;
		Ref<Texture2D>whiteTexture;
	};

	static Renderer2DStorage* s_data;

	void Renderer2D::init()
	{
		ME_PROFILE_FUNCTION();

		s_data = new Renderer2DStorage();
		s_data->quadVertexArray = VertexArray::create();
		float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};
		Ref<VertexBuffer> squareVB;
		squareVB.reset(VertexBuffer::create(squareVertices, sizeof(squareVertices)));

		BufferLayout squareLayout = {
			{ ShaderDataType::Float3, "a_position" },
			{ ShaderDataType::Float2, "a_textCoord" }
		};

		squareVB->setLayout(squareLayout);
		s_data->quadVertexArray->addVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		Ref<IndexBuffer> squareIB;
		squareIB.reset(IndexBuffer::create(squareIndices,
			sizeof(squareIndices) / sizeof(uint32_t)));
		s_data->quadVertexArray->setIndexBuffer(squareIB);

		s_data->whiteTexture = Texture2D::create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_data->whiteTexture->setData(&whiteTextureData, sizeof(uint32_t));

		s_data->textureShader = Shader::create("assets/shaders/Texture.glsl");
		s_data->textureShader->bind();
		s_data->textureShader->setInt("u_texture", 0);
	}

	void Renderer2D::shutdown()
	{
		ME_PROFILE_FUNCTION();

		delete s_data;
	}

	void Renderer2D::beginScene(OrthographicCamera const& camera)
	{
		ME_PROFILE_FUNCTION();

		s_data->textureShader->bind();
		s_data->textureShader->setMat4("u_viewProjection", camera.getViewProjectionMatrix());
	}

	void Renderer2D::endScene()
	{
		ME_PROFILE_FUNCTION();
	}

	void Renderer2D::drawQuad(glm::vec2 const& position, glm::vec2 const& size, glm::vec4 const& color)
	{
		drawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::drawQuad(glm::vec3 const& position, glm::vec2 const& size, glm::vec4 const& color)
	{
		ME_PROFILE_FUNCTION();

		s_data->textureShader->setFloat4("u_color", color);
		s_data->textureShader->setFloat("u_tilingFactor", 1.0f);
		s_data->whiteTexture->bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f),position) *
			glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});
		s_data->textureShader->setMat4("u_transform", transform);

		s_data->quadVertexArray->bind();
		RenderCommand::drawIndexed(s_data->quadVertexArray);
	}
	void Renderer2D::drawQuad(glm::vec2 const& position, glm::vec2 const& size, Ref<Texture2D> const& texture, float tilingFactor, glm::vec4 const& tintColor)
	{
		drawQuad({ position.x, position.y, 0.0f }, size, texture, tilingFactor, tintColor);
	}

	void Renderer2D::drawQuad(glm::vec3 const& position, glm::vec2 const& size, Ref<Texture2D> const& texture, float tilingFactor, glm::vec4 const& tintColor)
	{
		ME_PROFILE_FUNCTION();

		s_data->textureShader->setFloat4("u_color", tintColor);
		s_data->textureShader->setFloat("u_tilingFactor", tilingFactor);
		texture->bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		s_data->textureShader->setMat4("u_transform", transform);

		s_data->quadVertexArray->bind();
		RenderCommand::drawIndexed(s_data->quadVertexArray);
	}

	void Renderer2D::drawRotatedQuad(glm::vec2 const& position, glm::vec2 const& size, float rotation, glm::vec4 const& color)
	{
		drawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, color);
	}

	void Renderer2D::drawRotatedQuad(glm::vec3 const& position, glm::vec2 const& size, float rotation, glm::vec4 const& color)
	{
		ME_PROFILE_FUNCTION();

		s_data->textureShader->setFloat4("u_color", color);
		s_data->textureShader->setFloat("u_tilingFactor", 1.0f);
		s_data->whiteTexture->bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f }) * 
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_data->textureShader->setMat4("u_transform", transform);

		s_data->quadVertexArray->bind();
		RenderCommand::drawIndexed(s_data->quadVertexArray);
	}

	void Renderer2D::drawRotatedQuad(glm::vec2 const& position, glm::vec2 const& size, float rotation, Ref<Texture2D> const& texture, float tilingFactor, glm::vec4 const& tintColor)
	{
		drawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, texture, tilingFactor, tintColor);
	}

	void Renderer2D::drawRotatedQuad(glm::vec3 const& position, glm::vec2 const& size, float rotation, Ref<Texture2D> const& texture, float tilingFactor, glm::vec4 const& tintColor)
	{
		ME_PROFILE_FUNCTION();

		s_data->textureShader->setFloat4("u_color", tintColor);
		s_data->textureShader->setFloat("u_tilingFactor", tilingFactor);
		texture->bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f }) *
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		s_data->textureShader->setMat4("u_transform", transform);

		s_data->quadVertexArray->bind();
		RenderCommand::drawIndexed(s_data->quadVertexArray);
	}
}
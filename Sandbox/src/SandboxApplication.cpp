#include "MarsEngine.h"
#include "MarsEngine/Core/EntryPoint.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "imgui.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "Sandbox2D.h"

class ExampleLayer : public MarsEngine::Layer {

public:
	ExampleLayer() : Layer("Example"), m_cameraController(1920.0f / 1080.0f, true) {
		
		m_vertexArray = MarsEngine::VertexArray::create();

		float vertices[7 * 3] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};

		MarsEngine::Ref<MarsEngine::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(MarsEngine::VertexBuffer::create(vertices, sizeof(vertices)));

		MarsEngine::BufferLayout layout = {
			{MarsEngine::ShaderDataType::Float3, "a_position"},
			{MarsEngine::ShaderDataType::Float4, "a_color"}
		};

		vertexBuffer->setLayout(layout);
		m_vertexArray->addVertexBuffer(vertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };

		MarsEngine::Ref<MarsEngine::IndexBuffer> indexBuffer;
		indexBuffer.reset(MarsEngine::IndexBuffer::create(indices,
			sizeof(indices) / sizeof(uint32_t)));
		m_vertexArray->setIndexBuffer(indexBuffer);

		m_squareVA = MarsEngine::VertexArray::create();
		float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};
		MarsEngine::Ref<MarsEngine::VertexBuffer> squareVB;
		squareVB.reset(MarsEngine::VertexBuffer::create(squareVertices,
			sizeof(squareVertices)));

		MarsEngine::BufferLayout squareLayout = {
			{MarsEngine::ShaderDataType::Float3, "a_position"},
			{MarsEngine::ShaderDataType::Float2, "a_textCoord"}
		};

		squareVB->setLayout(squareLayout);
		m_squareVA->addVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		MarsEngine::Ref<MarsEngine::IndexBuffer> squareIB;
		squareIB.reset(MarsEngine::IndexBuffer::create(squareIndices,
			sizeof(squareIndices) / sizeof(uint32_t)));
		m_squareVA->setIndexBuffer(squareIB);

		
	}

	void onUpdate(MarsEngine::Timestep ts) override
	{
		m_cameraController.onUpdate(ts);
		
	}

	virtual void onImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_squareColor));
		ImGui::End();
	}

	void onEvent(MarsEngine::Event& e) override
	{
		m_cameraController.onEvent(e);
	}

private:
	MarsEngine::ShaderLibrary m_shaderLibrary;
	MarsEngine::Ref<MarsEngine::Shader> m_shader;
	MarsEngine::Ref<MarsEngine::VertexArray> m_vertexArray;

	MarsEngine::Ref<MarsEngine::Shader> m_flatColorShader;
	MarsEngine::Ref<MarsEngine::VertexArray> m_squareVA;

	MarsEngine::Ref<MarsEngine::Texture2D> m_texture, m_ChernoLogoTexture;

	MarsEngine::OrthographicCameraController m_cameraController;

	glm::vec3 m_squareColor = { 0.2f, 0.3f, 0.8f };
};

class Sandbox : public MarsEngine::Application {

public:
	Sandbox() {
		//pushLayer(new ExampleLayer());

		pushLayer(new Sandbox2D());
	}

	~Sandbox() {}

};

MarsEngine::Application* MarsEngine::createApplication() {
	return new Sandbox();
}
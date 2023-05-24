#include "Sandbox2D.h"
#include "imgui.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Platform/OpenGL/OpenGLShader.h"

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_cameraController(1920.0f / 1080.0f, true)
{
}

void Sandbox2D::onAttach()
{
	m_squareVA = MarsEngine::VertexArray::create();
	float squareVertices[5 * 4] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.5f,  0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f
	};
	MarsEngine::Ref<MarsEngine::VertexBuffer> squareVB;
	squareVB.reset(MarsEngine::VertexBuffer::create(squareVertices, sizeof(squareVertices)));

	MarsEngine::BufferLayout squareLayout = {
		{MarsEngine::ShaderDataType::Float3, "a_position"}
	};

	squareVB->setLayout(squareLayout);
	m_squareVA->addVertexBuffer(squareVB);

	uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
	MarsEngine::Ref<MarsEngine::IndexBuffer> squareIB;
	squareIB.reset(MarsEngine::IndexBuffer::create(squareIndices,
		sizeof(squareIndices) / sizeof(uint32_t)));
	m_squareVA->setIndexBuffer(squareIB);

	m_flatColorShader = MarsEngine::Shader::create("assets/shaders/FlatColor.glsl");

}

void Sandbox2D::onDetach()
{
}

void Sandbox2D::onUpdate(MarsEngine::Timestep ts)
{
	m_cameraController.onUpdate(ts);

	MarsEngine::RenderCommand::setClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	MarsEngine::RenderCommand::clear();

	MarsEngine::Renderer::beginScene(m_cameraController.getCamera());

	glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

	glm::vec4 redColor(0.8f, 0.2f, 0.3f, 1.0f);
	glm::vec4 blueColor(0.2f, 0.3f, 0.8f, 1.0f);

	std::dynamic_pointer_cast<MarsEngine::OpenGLShader>(m_flatColorShader)->bind();
	std::dynamic_pointer_cast<MarsEngine::OpenGLShader>(m_flatColorShader)->
		uploadUniformFloat4("u_color", m_squareColor);

	m_flatColorShader->bind();
	MarsEngine::Renderer::submit(m_flatColorShader, m_squareVA,
		glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

	MarsEngine::Renderer::endScene();
}

void Sandbox2D::onImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_squareColor));
	ImGui::End();
}

void Sandbox2D::onEvent(MarsEngine::Event& e)
{
	m_cameraController.onEvent(e);
}

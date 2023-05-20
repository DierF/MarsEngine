#include <MarsEngine.h>

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "imgui.h"

#include "Platform/OpenGL/OpenGLShader.h"

class ExampleLayer : public MarsEngine::Layer {

public:
	ExampleLayer() : Layer("Example"), m_camera(-1.6f, 1.6f, -0.9f, 0.9f), m_cameraPosition(0.0f) {
		
		m_vertexArray.reset(MarsEngine::VertexArray::create());

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
		indexBuffer.reset(MarsEngine::IndexBuffer::create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_vertexArray->setIndexBuffer(indexBuffer);

		m_squareVA.reset(MarsEngine::VertexArray::create());
		float squareVertices[4 * 3] = {
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
		squareIB.reset(MarsEngine::IndexBuffer::create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_squareVA->setIndexBuffer(squareIB);

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_position;
			layout(location = 1) in vec4 a_color;

			uniform mat4 u_viewProjection;
			uniform mat4 u_transform;

			out vec3 v_position;
			out vec4 v_color;

			void main() {
				v_position = a_position;
				v_color = a_color;
				gl_Position = u_viewProjection * u_transform * vec4(a_position, 1.0);
			}

		)";

		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;
			
			in vec3 v_position;
			in vec4 v_color;

			void main() {
				color = vec4(v_position * 0.5 + 0.5, 1.0);
				color = v_color;
			}

		)";

		m_shader.reset(MarsEngine::Shader::create(vertexSrc, fragmentSrc));

		std::string flatColorVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_position;

			uniform mat4 u_viewProjection;
			uniform mat4 u_transform;

			out vec3 v_position;

			void main() {
				v_position = a_position;
				gl_Position = u_viewProjection * u_transform * vec4(a_position, 1.0);
			}

		)";

		std::string flatColorFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;
			
			in vec3 v_position;

			uniform vec3 u_color;

			void main() {
				color = vec4(u_color, 1.0);
			}

		)";

		m_flatColorShader.reset(MarsEngine::Shader::create(flatColorVertexSrc, flatColorFragmentSrc));
	}

	void onUpdate(MarsEngine::Timestep ts) override
	{
		ME_TRACE("Delta time: {0}s, {1}ms", ts.getSeconds(), ts.getMilliseconds());

		if (MarsEngine::Input::isKeyPressed(ME_KEY_LEFT)) {
			m_cameraPosition.x -= m_cameraMoveSpeed * ts;
		}
		else if (MarsEngine::Input::isKeyPressed(ME_KEY_RIGHT)) {
			m_cameraPosition.x += m_cameraMoveSpeed * ts;
		}
		if (MarsEngine::Input::isKeyPressed(ME_KEY_DOWN)) {
			m_cameraPosition.y -= m_cameraMoveSpeed * ts;
		}
		else if (MarsEngine::Input::isKeyPressed(ME_KEY_UP)) {
			m_cameraPosition.y += m_cameraMoveSpeed * ts;
		}

		if (MarsEngine::Input::isKeyPressed(ME_KEY_A)) {
			m_cameraRotation += m_cameraRotationSpeed * ts;
		}
		else if (MarsEngine::Input::isKeyPressed(ME_KEY_D)) {
			m_cameraRotation -= m_cameraRotationSpeed * ts;
		}

		MarsEngine::RenderCommand::setClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		MarsEngine::RenderCommand::clear();

		m_camera.setPosition(m_cameraPosition);
		m_camera.setRotation(m_cameraRotation);

		MarsEngine::Renderer::beginScene(m_camera);
		
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		glm::vec4 redColor(0.8f, 0.2f, 0.3f, 1.0f);
		glm::vec4 blueColor(0.2f, 0.3f, 0.8f, 1.0f);

		std::dynamic_pointer_cast<MarsEngine::OpenGLShader>(m_flatColorShader)->bind();
		std::dynamic_pointer_cast<MarsEngine::OpenGLShader>(m_flatColorShader)->uploadUniformFloat3("u_color", m_squareColor);

		for (int y = 0; y < 20; ++y)
		{
			for (int x = 0; x < 20; ++x)
			{
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				MarsEngine::Renderer::submit(m_flatColorShader, m_squareVA, transform);
			}
		}
		

		MarsEngine::Renderer::submit(m_shader, m_vertexArray);

		MarsEngine::Renderer::endScene();
	}

	virtual void onImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_squareColor));
		ImGui::End();
	}

	void onEvent(MarsEngine::Event& event) override
	{

	}

private:
	MarsEngine::Ref<MarsEngine::Shader> m_shader;
	MarsEngine::Ref<MarsEngine::VertexArray> m_vertexArray;

	MarsEngine::Ref<MarsEngine::Shader> m_flatColorShader;
	MarsEngine::Ref<MarsEngine::VertexArray> m_squareVA;

	MarsEngine::OrthographicCamera m_camera;
	glm::vec3 m_cameraPosition;
	float m_cameraMoveSpeed = 5.0f;
	float m_cameraRotation = 0.0f;
	float m_cameraRotationSpeed = 3.14f;

	glm::vec3 m_squareColor = { 0.2f, 0.3f, 0.8f };
};

class Sandbox : public MarsEngine::Application {

public:
	Sandbox() {
		pushLayer(new ExampleLayer());
	}

	~Sandbox() {}

	
};

MarsEngine::Application* MarsEngine::createApplication() {
	return new Sandbox();
}
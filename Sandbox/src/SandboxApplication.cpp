#include <MarsEngine.h>

class ExampleLayer : public MarsEngine::Layer {

public:
	ExampleLayer() : Layer("Example"), m_camera(-1.6f, 1.6f, -0.9f, 0.9f), m_cameraPosition(0.0f) {
		
		m_vertexArray.reset(MarsEngine::VertexArray::create());

		float vertices[7 * 3] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};

		std::shared_ptr<MarsEngine::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(MarsEngine::VertexBuffer::create(vertices, sizeof(vertices)));

		MarsEngine::BufferLayout layout = {
			{MarsEngine::ShaderDataType::Float3, "a_position"},
			{MarsEngine::ShaderDataType::Float4, "a_color"}
		};

		vertexBuffer->setLayout(layout);
		m_vertexArray->addVertexBuffer(vertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };

		std::shared_ptr<MarsEngine::IndexBuffer> indexBuffer;
		indexBuffer.reset(MarsEngine::IndexBuffer::create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_vertexArray->setIndexBuffer(indexBuffer);

		m_squareVA.reset(MarsEngine::VertexArray::create());
		float squareVertices[4 * 3] = {
			-0.75f, -0.75f, 0.0f,
			 0.75f, -0.75f, 0.0f,
			 0.75f,  0.75f, 0.0f,
			-0.75f,  0.75f, 0.0f
		};
		std::shared_ptr<MarsEngine::VertexBuffer> squareVB;
		squareVB.reset(MarsEngine::VertexBuffer::create(squareVertices, sizeof(squareVertices)));

		MarsEngine::BufferLayout squareLayout = {
			{MarsEngine::ShaderDataType::Float3, "a_position"}
		};

		squareVB->setLayout(squareLayout);
		m_squareVA->addVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<MarsEngine::IndexBuffer> squareIB;
		squareIB.reset(MarsEngine::IndexBuffer::create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_squareVA->setIndexBuffer(squareIB);

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_position;
			layout(location = 1) in vec4 a_color;

			uniform mat4 u_viewProjection;

			out vec3 v_position;
			out vec4 v_color;

			void main() {
				v_position = a_position;
				v_color = a_color;
				gl_Position = u_viewProjection * vec4(a_position, 1.0);
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

		m_shader = std::make_unique<MarsEngine::Shader>(vertexSrc, fragmentSrc);

		std::string blueVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_position;

			uniform mat4 u_viewProjection;

			out vec3 v_position;

			void main() {
				v_position = a_position;
				gl_Position = u_viewProjection * vec4(a_position, 1.0);
			}

		)";

		std::string blueFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;
			
			in vec3 v_position;

			void main() {
				color = vec4(0.2, 0.3, 0.8, 0.1);
			}

		)";

		m_blueShader = std::make_unique<MarsEngine::Shader>(blueVertexSrc, blueFragmentSrc);
	}

	void onUpdate() override {
		if (MarsEngine::Input::isKeyPressed(ME_KEY_LEFT)) {
			m_cameraPosition.x -= m_cameraMoveSpeed;
		}
		else if (MarsEngine::Input::isKeyPressed(ME_KEY_RIGHT)) {
			m_cameraPosition.x += m_cameraMoveSpeed;
		}
		if (MarsEngine::Input::isKeyPressed(ME_KEY_DOWN)) {
			m_cameraPosition.y -= m_cameraMoveSpeed;
		}
		else if (MarsEngine::Input::isKeyPressed(ME_KEY_UP)) {
			m_cameraPosition.y += m_cameraMoveSpeed;
		}
		if (MarsEngine::Input::isKeyPressed(ME_KEY_A)) {
			m_cameraRotation += m_cameraRotationSpeed;
		}
		else if (MarsEngine::Input::isKeyPressed(ME_KEY_D)) {
			m_cameraRotation -= m_cameraRotationSpeed;
		}

		MarsEngine::RenderCommand::setClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		MarsEngine::RenderCommand::clear();

		m_camera.setPosition(m_cameraPosition);
		m_camera.setRotation(m_cameraRotation);

		MarsEngine::Renderer::beginScene(m_camera);

		MarsEngine::Renderer::submit(m_blueShader, m_squareVA);

		MarsEngine::Renderer::submit(m_shader, m_vertexArray);

		MarsEngine::Renderer::endScene();
	}

	virtual void onImGuiRender() override {

	}

	void onEvent(MarsEngine::Event& event) override {
	}

private:
	std::shared_ptr<MarsEngine::Shader> m_shader;
	std::shared_ptr<MarsEngine::VertexArray> m_vertexArray;

	std::shared_ptr<MarsEngine::Shader> m_blueShader;
	std::shared_ptr<MarsEngine::VertexArray> m_squareVA;

	MarsEngine::OrthographicCamera m_camera;
	glm::vec3 m_cameraPosition;
	float m_cameraMoveSpeed = 0.1f;
	float m_cameraRotation = 0.0f;
	float m_cameraRotationSpeed = 0.1f;
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
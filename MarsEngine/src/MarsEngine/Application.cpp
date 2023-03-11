#include "pch.h"
#include "Application.h"
#include "MarsEngine/Log.h"
#include "GLAD/glad.h"

#include "Input.h"

namespace MarsEngine {

#define BIND_EVENT_FUNC(x) std::bind(&x, this, std::placeholders::_1)

	Application* Application::s_instance = nullptr;

	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type) {

		switch (type)
		{
		case MarsEngine::ShaderDataType::Float:
			return GL_FLOAT;
		case MarsEngine::ShaderDataType::Float2:
			return GL_FLOAT;
		case MarsEngine::ShaderDataType::Float3:
			return GL_FLOAT;
		case MarsEngine::ShaderDataType::Float4:
			return GL_FLOAT;
		case MarsEngine::ShaderDataType::Matrix3:
			return GL_FLOAT;
		case MarsEngine::ShaderDataType::Matrix4:
			return GL_FLOAT;
		case MarsEngine::ShaderDataType::Int:
			return GL_INT;
		case MarsEngine::ShaderDataType::Int2:
			return GL_INT;
		case MarsEngine::ShaderDataType::Int3:
			return GL_INT;
		case MarsEngine::ShaderDataType::Int4:
			return GL_INT;
		case MarsEngine::ShaderDataType::Bool:
			return GL_BOOL;
		default:
			break;
		}
		return 0;
	}

	Application::Application() {
		ME_CORE_ASSERT(!s_instance, "Application already exists!");
		s_instance = this;

		m_window = std::unique_ptr<Window>(Window::create());
		m_window->setEventCallback(BIND_EVENT_FUNC(Application::onEvent));

		m_imGuiLayer = new ImGuiLayer();
		pushOverlay(m_imGuiLayer);

		glGenVertexArrays(1, &m_vertexArray);
		glBindVertexArray(m_vertexArray);

		float vertices[7 * 3] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};

		m_vertexBuffer.reset(VertexBuffer::create(vertices, sizeof(vertices)));

		{
			BufferLayout layout = {
				{ShaderDataType::Float3, "a_position"},
				{ShaderDataType::Float4, "a_color"}
			};

			m_vertexBuffer->setLayout(layout);
		}

		auto const& layout = m_vertexBuffer->getLayout();
		uint32_t index = 0;
		for (auto const& element : layout) {

			glEnableVertexAttribArray(index);
			glVertexAttribPointer(
				index,
				element.getComponentCount(),
				ShaderDataTypeToOpenGLBaseType(element.m_type),
				element.m_normalized ? GL_TRUE : GL_FALSE,
				layout.getStride(),
				(void const*)element.m_offset);
			++index;
		}

		uint32_t indices[3] = { 0, 1, 2 };
		m_indexBuffer.reset(IndexBuffer::create(indices, sizeof(indices) / sizeof(uint32_t)));

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_position;
			layout(location = 1) in vec4 a_color;

			out vec3 v_position;
			out vec4 v_color;


			void main() {
				v_position = a_position;
				v_color = a_color;
				gl_Position = vec4(a_position, 1.0);
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

		m_shader = std::make_unique<Shader>(vertexSrc, fragmentSrc);
	}

	Application::~Application() {}

	void Application::onEvent(Event& event) {
		EventDispatcher dispatcher(event);
		dispatcher.dispatch<WindowCloseEvent>(BIND_EVENT_FUNC(Application::onWindowClose));
		//ME_CORE_TRACE("{0}", event);
		for (auto iter = m_layerStack.end(); iter != m_layerStack.begin(); ) {
			(*--iter)->onEvent(event);
			if (event.isHandled()) {
				break;
			}
		}
	}

	void Application::pushLayer(Layer* layer) {
		m_layerStack.pushLayer(layer);
		layer->onAttach();
	}

	void Application::pushOverlay(Layer* overlay) {
		m_layerStack.pushOverlay(overlay);
		overlay->onAttach();
	}

	void Application::run() {

		while (m_running) {
			glClearColor(0.1, 0.1, 0.1, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			m_shader->bind();

			glBindVertexArray(m_vertexArray);
			glDrawElements(GL_TRIANGLES, m_indexBuffer->getCount(), GL_UNSIGNED_INT, nullptr);

			for (auto layer : m_layerStack) {
				layer->onUpdate();
			}

			m_imGuiLayer->begin();
			for (auto layer : m_layerStack) {
				layer->onImGuiRender();
			}
			m_imGuiLayer->end();

			m_window->onUpdate();
		}
	}

	bool Application::onWindowClose(WindowCloseEvent& event) {
		m_running = false;
		return true;
	}

}
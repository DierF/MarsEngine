#include "pch.h"
#include "Application.h"
#include "MarsEngine/Log.h"
#include "Input.h"
#include "MarsEngine/Renderer/Renderer.h"
#include "GLFW/glfw3.h"

namespace MarsEngine {

#define BIND_EVENT_FUNC(x) std::bind(&x, this, std::placeholders::_1)

	Application* Application::s_instance = nullptr;

	Application::Application()
	{
		ME_CORE_ASSERT(!s_instance, "Application already exists!");
		s_instance = this;

		m_window = std::unique_ptr<Window>(Window::create());
		m_window->setEventCallback(BIND_EVENT_FUNC(Application::onEvent));
		//m_window->setVSync(false);

		Renderer::init();

		m_imGuiLayer = new ImGuiLayer();
		pushOverlay(m_imGuiLayer);
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

		while (m_running)
		{
			float time = (float)glfwGetTime();
			Timestep timestep = time - m_lastFrameTime;
			m_lastFrameTime = time;
			for (auto layer : m_layerStack) {
				layer->onUpdate(timestep);
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
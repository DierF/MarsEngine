#include "pch.h"
#include "Application.h"
#include "MarsEngine/Log.h"
#include "GLAD/glad.h"

namespace MarsEngine {

#define BIND_EVENT_FUNC(x) std::bind(&x, this, std::placeholders::_1)

	Application::Application() {
		m_window = std::unique_ptr<Window>(Window::create());
		m_window->setEventCallback(BIND_EVENT_FUNC(Application::onEvent));
	}

	Application::~Application() {}

	void Application::onEvent(Event& event) {
		EventDispatcher dispatcher(event);
		dispatcher.dispatch<WindowCloseEvent>(BIND_EVENT_FUNC(Application::onWindowClose));
		ME_CORE_TRACE("{0}", event);
		for (auto iter = m_layerStack.end(); iter != m_layerStack.begin(); ) {
			(*--iter)->onEvent(event);
			if (event.isHandled()) {
				break;
			}
		}
	}

	void Application::pushLayer(Layer* layer) {
		m_layerStack.pushLayer(layer);
	}

	void Application::pushOverlay(Layer* overlay) {
		m_layerStack.pushOverlay(overlay);
	}

	void Application::run() {

		while (m_running) {
			glClearColor(1, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			for (auto layer : m_layerStack) {
				layer->onUpdate();
			}
			m_window->onUpdate();
		}
	}

	bool Application::onWindowClose(WindowCloseEvent& event) {
		m_running = false;
		return true;
	}

}
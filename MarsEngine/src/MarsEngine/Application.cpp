#include "pch.h"
#include "Application.h"
#include "MarsEngine/Event/ApplicationEvent.h"
#include "MarsEngine/Log.h"
#include "GLFW/glfw3.h"

namespace MarsEngine {

	Application::Application() {
		m_window = std::unique_ptr<Window>(Window::create());
	}

	Application::~Application() {}

	void Application::run() {
		WindowResizeEvent event(1280, 720);
		/*if (event.isInCategory(EventCategoryApplication)) {
			ME_TRACE(event);
		}
		if (event.isInCategory(EventCategoryInput)) {
			ME_TRACE(event);
		}*/

		while (m_running) {
			glClearColor(1, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			m_window->onUpdate();
		}
	}

}
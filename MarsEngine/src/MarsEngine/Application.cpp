#include "Application.h"
#include "MarsEngine/Event/ApplicationEvent.h"
#include "MarsEngine/Log.h"

namespace MarsEngine {

	Application::Application() {}

	Application::~Application() {}

	void Application::run() {
		WindowResizeEvent event(1000, 700);
		ME_TRACE(event);

		while (true);
	}

}
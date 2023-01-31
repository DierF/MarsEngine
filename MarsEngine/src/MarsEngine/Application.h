#pragma once

#include "Core.h"
#include "Event/Event.h"
#include "Window.h"

namespace MarsEngine {

	class ME_API Application
	{
	public:
		Application();

		virtual ~Application();

		void run();

	private:
		std::unique_ptr<Window> m_window;
		bool m_running = true;
	};

	//to be defined in Client
	Application* createApplication();

}

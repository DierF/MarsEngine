#pragma once

#include "Core.h"
#include "Event/Event.h"
#include "MarsEngine/Event/ApplicationEvent.h"
#include "Window.h"
#include "LayerStack.h"

namespace MarsEngine {

	class ME_API Application
	{
	public:
		Application();

		virtual ~Application();

		void run();

		void onEvent(Event& event);

		void pushLayer(Layer* layer);

		void pushOverlay(Layer* overlay);

	private:
		bool onWindowClose(WindowCloseEvent& event);

		std::unique_ptr<Window> m_window;
		bool m_running = true;
		LayerStack m_layerStack;
	};

	//to be defined in Client
	Application* createApplication();

}

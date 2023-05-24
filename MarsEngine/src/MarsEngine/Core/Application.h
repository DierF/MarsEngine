#pragma once

#include "Core.h"
#include "MarsEngine/Event/Event.h"
#include "MarsEngine/Event/ApplicationEvent.h"
#include "Window.h"
#include "LayerStack.h"
#include "MarsEngine/ImGui/ImGuiLayer.h"
#include "MarsEngine/Core/Timestep.h"

namespace MarsEngine {

	class Application
	{
	public:
		Application();

		virtual ~Application();

		void run();

		void onEvent(Event& event);

		void pushLayer(Layer* layer);

		void pushOverlay(Layer* overlay);

		inline static Application& get() {
			return *s_instance;
		}

		inline Window& getWindow() {
			return *m_window;
		}

	private:
		bool onWindowClose(WindowCloseEvent& e);

		bool onWindowResize(WindowResizeEvent& e);

	private:
		std::unique_ptr<Window> m_window;
		ImGuiLayer* m_imGuiLayer;
		bool m_running = true;
		bool m_minimized = false;
		LayerStack m_layerStack;
		float m_lastFrameTime = 0.0f;

	private:
		static Application* s_instance;
	};

	//to be defined in Client
	Application* createApplication();

}

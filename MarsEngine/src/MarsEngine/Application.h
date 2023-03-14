#pragma once

#include "Core.h"
#include "Event/Event.h"
#include "MarsEngine/Event/ApplicationEvent.h"
#include "Window.h"
#include "LayerStack.h"
#include "ImGui/ImGuiLayer.h"
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
		bool onWindowClose(WindowCloseEvent& event);

	private:
		std::unique_ptr<Window> m_window;
		ImGuiLayer* m_imGuiLayer;
		bool m_running = true;
		LayerStack m_layerStack;
		float m_lastFrameTime = 0.0f;

	private:
		static Application* s_instance;
	};

	//to be defined in Client
	Application* createApplication();

}

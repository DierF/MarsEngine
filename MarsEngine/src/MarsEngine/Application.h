#pragma once

#include "Core.h"
#include "Event/Event.h"
#include "MarsEngine/Event/ApplicationEvent.h"
#include "Window.h"
#include "LayerStack.h"
#include "ImGui/ImGuiLayer.h"
#include "Renderer/Shader.h"
#include "Renderer/Buffer.h"

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

		inline static Application& get() {
			return *s_instance;
		}

		inline Window& getWindow() {
			return *m_window;
		}

	private:
		bool onWindowClose(WindowCloseEvent& event);

		std::unique_ptr<Window> m_window;
		ImGuiLayer* m_imGuiLayer;
		bool m_running = true;
		LayerStack m_layerStack;

		unsigned int m_vertexArray;

		std::unique_ptr<Shader> m_shader;
		std::unique_ptr<VertexBuffer> m_vertexBuffer;
		std::unique_ptr<IndexBuffer> m_indexBuffer;


	private:
		static Application* s_instance;
	};

	//to be defined in Client
	Application* createApplication();

}

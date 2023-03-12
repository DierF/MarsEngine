#pragma once

#include "Core.h"
#include "Event/Event.h"
#include "MarsEngine/Event/ApplicationEvent.h"
#include "Window.h"
#include "LayerStack.h"
#include "ImGui/ImGuiLayer.h"
#include "Renderer/Shader.h"
#include "Renderer/Buffer.h"
#include "Renderer/VertexArray.h"

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

		std::shared_ptr<Shader> m_shader;
		std::shared_ptr<VertexArray> m_vertexArray;

		std::shared_ptr<Shader> m_blueShader;
		std::shared_ptr<VertexArray> m_squareVA;

	private:
		static Application* s_instance;
	};

	//to be defined in Client
	Application* createApplication();

}

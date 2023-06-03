#include "pch.h"
#include "Application.h"
#include "MarsEngine/Core/Log.h"
#include "Input.h"
#include "MarsEngine/Renderer/Renderer.h"
#include "GLFW/glfw3.h"
#include "glad/glad.h"

namespace MarsEngine {

#define BIND_EVENT_FUNC(x) std::bind(&x, this, std::placeholders::_1)

	Application* Application::s_instance = nullptr;

	Application::Application()
	{
		ME_PROFILE_FUNCTION();

		ME_CORE_ASSERT(!s_instance, "Application already exists!");
		s_instance = this;

		m_window = std::unique_ptr<Window>(Window::create());
		m_window->setEventCallback(BIND_EVENT_FUNC(Application::onEvent));
		//m_window->setVSync(false);

		Renderer::init();

		m_imGuiLayer = new ImGuiLayer();
		pushOverlay(m_imGuiLayer);
	}

	Application::~Application()
	{
		ME_PROFILE_FUNCTION();
	}

	void Application::onEvent(Event& event)
	{
		ME_PROFILE_FUNCTION();

		EventDispatcher dispatcher(event);
		dispatcher.dispatch<WindowCloseEvent>(BIND_EVENT_FUNC(Application::onWindowClose));
		dispatcher.dispatch<WindowResizeEvent>(BIND_EVENT_FUNC(Application::onWindowResize));
		//ME_CORE_TRACE("{0}", event);
		for (auto iter = m_layerStack.end(); iter != m_layerStack.begin(); )
		{
			(*--iter)->onEvent(event);
			if (event.isHandled()) {
				break;
			}
		}
	}

	void Application::pushLayer(Layer* layer)
	{
		ME_PROFILE_FUNCTION();

		m_layerStack.pushLayer(layer);
		layer->onAttach();
	}

	void Application::pushOverlay(Layer* overlay)
	{
		ME_PROFILE_FUNCTION();

		m_layerStack.pushOverlay(overlay);
		overlay->onAttach();
	}

	void Application::run()
	{
		ME_PROFILE_FUNCTION();

		while (m_running)
		{
			ME_PROFILE_SCOPE("Runloop");

			float time = (float)glfwGetTime();
			Timestep timestep = time - m_lastFrameTime;
			ME_CORE_TRACE("Frametime: {}s, FPS: {}", timestep, (uint32_t)(1.0f / timestep.getSeconds()));
			m_lastFrameTime = time;
			if (!m_minimized)
			{
				{
					ME_PROFILE_SCOPE("LayerStack onUpdate");

					for (auto layer : m_layerStack)
					{
						layer->onUpdate(timestep);
					}
				}
			}
			m_imGuiLayer->begin();
			{
				ME_PROFILE_SCOPE("LayerStack onImGuiRender");
				for (auto layer : m_layerStack)
				{
					layer->onImGuiRender();
				}
			}
			m_imGuiLayer->end();

			m_window->onUpdate();
		}
	}

	void Application::close()
	{
		m_running = false;
	}

	bool Application::onWindowClose(WindowCloseEvent& e)
	{
		m_running = false;
		return true;
	}

	bool Application::onWindowResize(WindowResizeEvent& e)
	{
		ME_PROFILE_FUNCTION();

		if (e.getWidth() == 0 || e.getHeight() == 0)
		{
			m_minimized = true;
			return false;
		}

		m_minimized = false;
		Renderer::onWindowResize(e.getWidth(), e.getHeight());
		return false;
	}

}
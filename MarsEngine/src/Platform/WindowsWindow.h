#pragma once

#include <MarsEngine/Window.h>

#include <GLFW/glfw3.h>

namespace MarsEngine {

	class WindowsWindow : public Window {

	public:
		WindowsWindow(const WindowProps& props);

		virtual ~WindowsWindow();

		void onUpdate() override;

		inline unsigned int getWidth() const override {
			return m_data.m_width;
		}

		inline unsigned int getHeight() const override {
			return m_data.m_height;
		}

		inline void setEventCallback(const EventCallbackFunc& callback) override {
			m_data.m_eventCallback = callback;
		}

		void setVSync(bool enabled) override;

		bool isVSync() const override;

	private:
		virtual void init(const WindowProps& props);

		virtual void shutdown();

		GLFWwindow* m_window;

		struct WindowData {

			std::string m_title;
			unsigned int m_width, m_height;
			bool VSync;

			EventCallbackFunc m_eventCallback;
		};

		WindowData m_data;
	};
}
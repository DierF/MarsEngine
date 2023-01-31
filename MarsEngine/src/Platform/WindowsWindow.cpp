#include "pch.h"

#include "WindowsWindow.h"

namespace MarsEngine {

	static bool s_GLFWInitialized = false;

	Window* Window::create(const WindowProps& props) {
		return new WindowsWindow(props);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props) {
		init(props);
	}

	WindowsWindow::~WindowsWindow() {
		shutdown();
	}

	void WindowsWindow::init(const WindowProps& props) {
		m_data.m_title = props.m_title;
		m_data.m_width = props.m_width;
		m_data.m_height = props.m_height;

		ME_CORE_INFO("Creating window {0} ({1}, {2})", props.m_title, props.m_width, props.m_height);

		if (!s_GLFWInitialized) {
			int success = glfwInit();
			ME_CORE_ASSERT(success, "Could not intialize GLFW!\n");
			s_GLFWInitialized = true;
		}

		m_window = glfwCreateWindow((int)props.m_width, (int)props.m_height, m_data.m_title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_window);
		glfwSetWindowUserPointer(m_window, &m_data);
		setVSync(true);
	}

	void WindowsWindow::shutdown() {
		glfwDestroyWindow(m_window);
	}

	void WindowsWindow::onUpdate() {
		glfwPollEvents();
		glfwSwapBuffers(m_window);
	}

	void WindowsWindow::setVSync(bool enabled) {
		if (enabled) {
			glfwSwapInterval(1);
		}
		else {
			glfwSwapInterval(0);
		}
		m_data.VSync = enabled;
	}

	bool WindowsWindow::isVSync() const {
		return m_data.VSync;
	}
}
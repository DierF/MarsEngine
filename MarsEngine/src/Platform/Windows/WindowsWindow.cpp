#include "pch.h"

#include "WindowsWindow.h"

#include "MarsEngine/Event/ApplicationEvent.h"
#include "MarsEngine/Event/KeyEvent.h"
#include "MarsEngine/Event/MouseEvent.h"

#include "Platform/OpenGL/OpenGLContext.h"

#include "GLAD/glad.h"


namespace MarsEngine {

	static bool s_GLFWInitialized = false;

	static void GLFWErrorCallback(int error, char const* description) {
		ME_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
	}

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
			glfwSetErrorCallback(GLFWErrorCallback);
			s_GLFWInitialized = true;
		}

		m_window = glfwCreateWindow((int)props.m_width, (int)props.m_height, m_data.m_title.c_str(), nullptr, nullptr);

		m_context = new OpenGLContext(m_window);
		m_context->init();

		glfwSetWindowUserPointer(m_window, &m_data);
		setVSync(true);

		glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int width, int height) {
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				data.m_width = width;
				data.m_height = height;
				WindowResizeEvent event(width, height);
				data.m_eventCallback(event);
			});

		glfwSetWindowCloseCallback(m_window,
			[](GLFWwindow* window) {
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				WindowCloseEvent event;
				data.m_eventCallback(event);
			});

		glfwSetKeyCallback(m_window,
			[](GLFWwindow* window, int key, int scancode, int action, int mods) {
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				switch (action) {
					case GLFW_PRESS: {
						KeyPressedEvent event(key, 0);
						data.m_eventCallback(event);
						break;
					}
					case GLFW_RELEASE: {
						KeyReleasedEvent event(key);
						data.m_eventCallback(event);
						break;
					}
					case GLFW_REPEAT: {
						KeyPressedEvent event(key, 1);
						data.m_eventCallback(event);
						break;
					}
				}
			});

		glfwSetCharCallback(m_window,
			[](GLFWwindow* window, unsigned int keycode) {
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				KeyTypedEvent event(keycode);
				data.m_eventCallback(event);
			});

		glfwSetMouseButtonCallback(m_window,
			[](GLFWwindow* window, int button, int action, int mods) {
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				switch (action) {
					case GLFW_PRESS: {
						MouseButtonPressedEvent event(button);
						data.m_eventCallback(event);
						break;
					}
					case GLFW_RELEASE: {
						MouseButtonReleasedEvent event(button);
						data.m_eventCallback(event);
						break;
					}
				}
			});

		glfwSetScrollCallback(m_window,
			[](GLFWwindow* window, double offsetX, double offsetY) {
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				MouseScrolledEvent event((float)offsetX, (float)offsetY);
				data.m_eventCallback(event);
			});

		glfwSetCursorPosCallback(m_window,
			[](GLFWwindow* window, double posX, double posY) {
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				MouseMovedEvent event((float)posX, (float)posY);
				data.m_eventCallback(event);
			});
	}

	void WindowsWindow::shutdown() {
		glfwDestroyWindow(m_window);
	}

	void WindowsWindow::onUpdate() {
		glfwPollEvents();
		m_context->swapBuffers();
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
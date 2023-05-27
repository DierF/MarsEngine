#include "pch.h"
#include "OpenGLContext.h"

#include "GLFW/glfw3.h"
#include "glad/glad.h"

namespace MarsEngine {

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_windowHandle(windowHandle)
	{
	}

	void OpenGLContext::init()
	{
		ME_PROFILE_FUNCTION();

		glfwMakeContextCurrent(m_windowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		ME_CORE_ASSERT(status, "Failed to initialize GLAD!\n");

		//ME_CORE_INFO("OpenGL Info:");
		//ME_CORE_INFO("Vendor: {0}", glGetString(GL_VENDOR));
		//ME_CORE_INFO("Renderer: {0}", glGetString(GL_RENDERER));
		//ME_CORE_INFO("Version: {0}", glGetString(GL_VERSION));
	}

	void OpenGLContext::swapBuffers()
	{
		ME_PROFILE_FUNCTION();

		glfwSwapBuffers(m_windowHandle);
	}
}
#include "pch.h"
#include "WindowsInput.h"

#include "GLFW/glfw3.h"
#include "MarsEngine/Application.h"

namespace MarsEngine {

	Input* Input::s_instance = new WindowsInput();

	bool WindowsInput::isKeyPressedImpl(int keyCode)
	{
		auto window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());
		auto state = glfwGetKey(window, keyCode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}
	bool WindowsInput::isMouseButtonPressedImpl(int button)
	{
		auto window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());
		auto state = glfwGetMouseButton(window, button);
		return state == GLFW_PRESS;
	}
	std::pair<float, float> WindowsInput::getMousePositionImpl()
	{
		auto window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		return { (float)xpos, (float)ypos };
	}
	float WindowsInput::getMouseXImpl()
	{
		auto [x, y] = getMousePositionImpl();
		return x;
	}
	float WindowsInput::getMouseYImpl()
	{
		auto [x, y] = getMousePositionImpl();
		return y;
	}
}

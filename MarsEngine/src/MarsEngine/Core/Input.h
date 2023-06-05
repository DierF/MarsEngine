#pragma once

#include "Core.h"

namespace MarsEngine
{
	class Input
	{
	public:
		static bool isKeyPressed(int keyCode);

		static bool isMouseButtonPressed(int button);

		static std::pair<float, float> getMousePosition();

		static float getMouseX();

		static float getMouseY();

	};
}
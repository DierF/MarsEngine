#pragma once

#include "MarsEngine/Core/Input.h"

namespace MarsEngine {

	class WindowsInput : public Input {

	protected:
		virtual bool isKeyPressedImpl(int keyCode) override;

		virtual bool isMouseButtonPressedImpl(int button) override;

		virtual std::pair<float, float> getMousePositionImpl() override;

		virtual float getMouseXImpl() override;

		virtual float getMouseYImpl() override;
	};
}
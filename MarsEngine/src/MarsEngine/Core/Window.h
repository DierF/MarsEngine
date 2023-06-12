#pragma once

#include "pch.h"
#include "Core.h"
#include "MarsEngine/Event/Event.h"

namespace MarsEngine
{
	struct WindowProps
	{
		std::string m_title;
		uint32_t m_width, m_height;

		WindowProps(const std::string& title = "Mars Engine",
			uint32_t width = 1920, uint32_t height = 1080)
			:m_title(title), m_width(width), m_height(height) {}
	};

	class ME_API Window
	{
	public:
		using EventCallbackFunc = std::function<void(Event&)>;

		virtual ~Window() {}

		virtual void onUpdate() = 0;

		virtual uint32_t getWidth() const = 0;

		virtual uint32_t getHeight() const = 0;

		virtual void setEventCallback(const EventCallbackFunc& callback) = 0;

		virtual void setVSync(bool enabled) = 0;

		virtual bool isVSync() const = 0;

		virtual void* getNativeWindow() const = 0;

		static Window* create(const WindowProps& props = WindowProps());
	};
}
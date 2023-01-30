#pragma once

#include "Event.h"

#include <sstream>

namespace MarsEngine {

	class ME_API MouseMovedEvent : public Event {
		
	public:
		MouseMovedEvent(float x, float y) : m_mouseX(x), m_mouseY(y) {}

		inline float getX() const {
			return m_mouseX;
		}

		inline float getY() const {
			return m_mouseY;
		}

		std::string toString() const override {
			std::stringstream ss;
			ss << "MouseMovedEvent: " << m_mouseX << ", " << m_mouseY << "\n";
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseMoved)
		
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

	private:
		float m_mouseX, m_mouseY;
	};

	class ME_API MouseScrolledEvent : public Event {

	public:
		MouseScrolledEvent(float x, float y) : m_offsetX(x), m_offsetY(y) {}

		inline float getOffsetX() const {
			return m_offsetX;
		}

		inline float getOffsetY() const {
			return m_offsetY;
		}

		std::string toString() const override {
			std::stringstream ss;
			ss << "MouseScrolledEvent: " << m_offsetX << ", " << m_offsetY << "\n";
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseScrolled)
		
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

	private:
		float m_offsetX, m_offsetY;
	};

	class ME_API MouseButtonEvent : public Event {

	public:
		inline int getMouseButton() const {
			return m_button;
		}

		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

	protected:
		MouseButtonEvent(int button) : m_button(button) {}

		int m_button;
	};

	class ME_API MouseButtonPressedEvent : public MouseButtonEvent {

	public:
		MouseButtonPressedEvent(int button) : MouseButtonEvent(button) {}

		std::string toString() const override {
			std::stringstream ss;
			ss << "MouseButtonPressedEvent: " << m_button << "\n";
			return ss.str();
		}
		
		EVENT_CLASS_TYPE(MouseButtonPressed)
	};

	class ME_API MouseButtonReleasedEvent : public MouseButtonEvent {

	public:
		MouseButtonReleasedEvent(int button) : MouseButtonEvent(button) {}

		std::string toString() const override {
			std::stringstream ss;
			ss << "MouseButtonReleasedEvent: " << m_button << "\n";
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonReleased)
	};
}
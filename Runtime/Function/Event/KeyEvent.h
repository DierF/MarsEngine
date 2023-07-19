#pragma once

#include "Event.h"

namespace MarsEngine {

	class ME_API KeyEvent : public Event {
	
	public:
		inline int getKeyCode() {
			return m_keyCode;
		}

		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)

	protected:
		KeyEvent(int keyCode) : m_keyCode(keyCode) {}

		int m_keyCode;
	};

	class ME_API KeyPressedEvent : public KeyEvent {

	public:
		KeyPressedEvent(int keycode, int repeatCount) : KeyEvent(keycode), m_repeatCount(repeatCount) {}

		inline int getRepeatCount() const {
			return m_repeatCount;
		}

		std::string toString() const override {
			std::stringstream ss;
			ss << "KeyPressedEvent: " << m_keyCode << "(" << m_repeatCount << " repeats)\n";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed)

	private:
		int m_repeatCount;
	};

	class ME_API KeyReleasedEvent : public KeyEvent {

	public:
		KeyReleasedEvent(int keycode) : KeyEvent(keycode) {}

		std::string toString() const override {
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << m_keyCode << "\n";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased)
	};

	class ME_API KeyTypedEvent : public KeyEvent {

	public:
		KeyTypedEvent(int keycode) : KeyEvent(keycode) {}

		std::string toString() const override {
			std::stringstream ss;
			ss << "KeyTypedEvent: " << m_keyCode << "\n";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyTyped)
	};
}
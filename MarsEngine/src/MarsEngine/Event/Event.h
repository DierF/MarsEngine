#pragma once

#include "MarsEngine/Core.h"

#include "pch.h"

namespace MarsEngine {

	enum class EventType {
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	enum EventCategory {
		None = 0,
		EventCategoryApplication	= BIT(0),
		EventCategoryInput			= BIT(1),
		EventCategoryKeyboard		= BIT(2),
		EventCategoryMouse			= BIT(3),
		EventCategoryMouseButton	= BIT(4)
	};

#define EVENT_CLASS_TYPE(type) static EventType getStaticType() {return EventType::##type; }\
								virtual EventType getEventType() const override {return getStaticType(); }\
								virtual const char* getName() const override {return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int getCategoryFlags() const override {return category; }

	class ME_API Event {

		friend class EventDispatcher;

	public:
		virtual EventType getEventType() const = 0;

		virtual const char* getName() const = 0;

		virtual int getCategoryFlags() const = 0;

		virtual std::string toString() const {
			return getName();
		}

		inline bool isHandled() const {
			return m_handled;
		}

		inline bool isInCategory(EventCategory category) {
			return getCategoryFlags() & category;
		}

	protected:
		bool m_handled = false;
	};

	class EventDispatcher {

		template<typename T>
		using EventFunc = std::function<bool(T&)>;

	public:
		EventDispatcher(Event& event) : m_event(event) {}

		template<typename T>
		bool dispatch(EventFunc<T> func) {
			if (m_event.getEventType() == T::getStaticType()) {
				m_event.m_handled = func(*(T*)&m_event);
				return true;
			}
		}

	private:
		Event& m_event;
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e) {
		return os << e.toString();
	}

}
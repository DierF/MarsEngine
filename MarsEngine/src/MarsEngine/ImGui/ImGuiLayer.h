#pragma once

#include "MarsEngine/Core/Layer.h"
#include "MarsEngine/Event/KeyEvent.h"
#include "MarsEngine/Event/MouseEvent.h"
#include "MarsEngine/Event/ApplicationEvent.h"

namespace MarsEngine
{
	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();

		~ImGuiLayer();

		virtual void onAttach() override;

		virtual void onDetach() override;

		virtual void onEvent(Event& e) override;

		void begin();

		void end();

		void blockEvents(bool block) { m_blockEvents = block; }

		void setDarkThemeColors();

	private:
		bool m_blockEvents = true;

		float m_time = 0.0f;
	};
}
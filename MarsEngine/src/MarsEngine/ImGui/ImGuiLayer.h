#pragma once

#include "MarsEngine/Core/Layer.h"
#include "MarsEngine/Event/KeyEvent.h"
#include "MarsEngine/Event/MouseEvent.h"
#include "MarsEngine/Event/ApplicationEvent.h"

namespace MarsEngine {

	class ME_API ImGuiLayer : public Layer {

	public:
		ImGuiLayer();

		~ImGuiLayer();

		virtual void onAttach() override;

		virtual void onDetach() override;

		void begin();

		void end();

	private:
		float m_time = 0.0f;
	};
}
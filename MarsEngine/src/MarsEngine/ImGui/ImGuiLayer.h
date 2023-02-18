#pragma once

#include "MarsEngine/Layer.h"

namespace MarsEngine {

	class ME_API ImGuiLayer : public Layer {

	public:
		ImGuiLayer();

		~ImGuiLayer();


		void onAttach();

		void onDetch();

		void onUpdate();

		void onEvent(Event& event);

	private:
		float m_time = 0.0f;
	};
}
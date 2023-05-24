#pragma once

#include "MarsEngine/Core/Core.h"
#include "MarsEngine/Event/Event.h"
#include "MarsEngine/Core/Timestep.h"

namespace MarsEngine {

	class ME_API Layer {

	public:
		Layer(std::string const& name = "Layer");

		virtual ~Layer();

		virtual void onAttach() {}

		virtual void onDetach() {}

		virtual void onUpdate(Timestep ts) {}

		virtual void onImGuiRender() {}

		virtual void onEvent(Event& event) {}

		inline std::string const& getName() const {
			return m_debugName;
		}

	protected:
		std::string m_debugName;
	};
}
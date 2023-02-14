#pragma once

#include "MarsEngine/Core.h"
#include "MarsEngine/Event/Event.h"

namespace MarsEngine {

	class ME_API Layer {

	public:
		Layer(std::string const& name = "Layer");

		virtual ~Layer();

		virtual void onAttach() {}

		virtual void onDetch() {}

		virtual void onUpdate() {}

		virtual void onEvent(Event& event) {}

		inline std::string const& getName() const {
			return m_debugName;
		}

	protected:
		std::string m_debugName;
	};
}
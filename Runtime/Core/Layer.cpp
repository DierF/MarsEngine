#include "pch.h"
#include "MarsEngine/Core/Layer.h"

namespace MarsEngine {

	Layer::Layer(std::string const& debugName) : m_debugName(debugName) {}

	Layer::~Layer() {}
}
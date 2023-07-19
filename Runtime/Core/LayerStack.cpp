#include "pch.h"
#include "LayerStack.h"

namespace MarsEngine
{

	LayerStack::LayerStack()
	{
	}

	LayerStack::~LayerStack()
	{
		for (Layer* layer : m_layers)
		{
			delete layer;
		}
	}

	void LayerStack::pushLayer(Layer* layer)
	{
		m_layers.emplace(m_layers.begin() + m_layerInsertIndex, layer);
		++m_layerInsertIndex;
	}

	void LayerStack::pushOverlay(Layer* overlay)
	{
		m_layers.emplace_back(overlay);
	}

	void LayerStack::popLayer(Layer* layer)
	{
		auto iter = std::find(m_layers.begin(), m_layers.end(), layer);
		if (iter != m_layers.begin() + m_layerInsertIndex)
		{
			layer->onDetach();
			m_layers.erase(iter);
			--m_layerInsertIndex;
		}
	}

	void LayerStack::popOverlay(Layer* overlay)
	{
		auto iter = std::find(m_layers.begin(), m_layers.end(), overlay);
		if (iter != m_layers.end())
		{
			overlay->onDetach();
			m_layers.erase(iter);
		}
	}
}
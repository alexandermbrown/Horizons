#include "lipch.h"
#include "LayerStack.h"

namespace li
{
	LayerStack::LayerStack()
		: m_Layers(), m_LayerInsertIndex(0)
	{
	}

	LayerStack::~LayerStack()
	{
	}

	void LayerStack::PushLayer(Layer* layer)
	{
		m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, layer);
		m_LayerInsertIndex++;
	}

	void LayerStack::PushOverlay(Layer* overlay)
	{
		m_Layers.emplace_back(overlay);
	}

	void LayerStack::PopLayer(Layer* layer)
	{
		auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);
		LI_CORE_ASSERT(it != m_Layers.end(), "Layer does not exist!");
		layer->OnDetach();
		m_Layers.erase(it);
		m_LayerInsertIndex--;
	}

	void LayerStack::PopOverlay(Layer* overlay)
	{
		auto it = std::find(m_Layers.begin(), m_Layers.end(), overlay);
		LI_CORE_ASSERT(it != m_Layers.end(), "Layer does not exist!");
		overlay->OnDetach();
		m_Layers.erase(it);
	}

	void LayerStack::Clear()
	{
		for (int i = (int)m_Layers.size() - 1; i >= 0; --i)
		{
			m_Layers[i]->OnDetach();
			m_Layers.pop_back();
		}
	}
}
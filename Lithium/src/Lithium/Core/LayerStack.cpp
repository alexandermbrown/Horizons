#include "lipch.h"
#include "LayerStack.h"

namespace Li
{
	LayerStack::LayerStack()
		: m_BottomOverlay(m_Layers.end())
	{
	}

	LayerStack::~LayerStack()
	{
	}

	void LayerStack::PushLayer(Unique<Layer> layer)
	{
#ifdef LI_ENABLE_ASSERTS
		LayerList::iterator it = std::find_if(m_Layers.begin(), m_Layers.end(),
			[&layer](const Unique<Layer>& other_layer) -> bool { return layer->GetName() == other_layer->GetName(); });

		LI_CORE_ASSERT(it == m_Layers.end(), "Layer already inserted!");
#endif
		m_Layers.emplace(m_BottomOverlay, std::move(layer));
	}

	void LayerStack::PushOverlay(Unique<Layer> overlay)
	{
#ifdef LI_ENABLE_ASSERTS
		LayerList::iterator it = std::find_if(m_Layers.begin(), m_Layers.end(),
			[&overlay](const Unique<Layer>& other_overlay) -> bool { return overlay->GetName() == other_overlay->GetName(); });

		LI_CORE_ASSERT(it == m_Layers.end(), "Overlay already inserted!");
#endif
		m_Layers.emplace_back(std::move(overlay));
	}

	void LayerStack::PopLayer(const std::string_view& layer_name)
	{
		LayerList::iterator it = std::find_if(m_Layers.begin(), m_Layers.end(),
			[&layer_name](const Unique<Layer>& layer) -> bool { return layer->GetName() == layer_name; });
		LI_CORE_ASSERT(it != m_Layers.end(), "Layer does not exist!");

		m_Layers.erase(it);
	}

	void LayerStack::PopOverlay(const std::string_view& overlay_name)
	{
		LayerList::iterator it = std::find_if(m_Layers.begin(), m_Layers.end(),
			[&overlay_name](const Unique<Layer>& layer) -> bool { return layer->GetName() == overlay_name; });
		LI_CORE_ASSERT(it != m_Layers.end(), "Overlay does not exist!");

		if (it == m_BottomOverlay)
			++m_BottomOverlay;

		m_Layers.erase(it);
	}

	Layer* LayerStack::GetLayer(const std::string_view& name)
	{
		LayerList::iterator it = std::find_if(m_Layers.begin(), m_Layers.end(),
			[&name](const Unique<Layer>& layer) -> bool { return layer->GetName() == name; });
		LI_CORE_ASSERT(it != m_Layers.end(), "Overlay does not exist!");
		
		return (*it).get();
	}

	void LayerStack::Clear()
	{
		m_Layers.clear();
	}
}

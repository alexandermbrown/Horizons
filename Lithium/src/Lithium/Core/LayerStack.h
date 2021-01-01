#pragma once

#include "Lithium/Core/Core.h"
#include "Layer.h"

#include <list>
#include <string_view>

namespace Li
{
	class LayerStack
	{
	public:
		LayerStack();
		~LayerStack();

		void PushLayer(Unique<Layer> layer);
		void PushOverlay(Unique<Layer> overlay);
		void PopLayer(const std::string_view& layer_name);
		void PopOverlay(const std::string_view& overlay_name);

		Layer* GetLayer(const std::string_view& name);

		void Clear();

		using LayerList = std::list<Unique<Layer>>;

		LayerList::iterator begin() { return m_Layers.begin(); }
		LayerList::iterator end() { return m_Layers.end(); }
		LayerList::reverse_iterator rbegin() { return m_Layers.rbegin(); }
		LayerList::reverse_iterator rend() { return m_Layers.rend(); }

		LayerList::const_iterator begin() const { return m_Layers.begin(); }
		LayerList::const_iterator end()	const { return m_Layers.end(); }
		LayerList::const_reverse_iterator rbegin() const { return m_Layers.rbegin(); }
		LayerList::const_reverse_iterator rend() const { return m_Layers.rend(); }

	private:
		LayerList m_Layers;
		LayerList::iterator m_BottomOverlay;
	};
}

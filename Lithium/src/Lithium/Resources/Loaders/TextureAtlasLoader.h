#pragma once

#include "Lithium/Renderer/TextureAtlas.h"

#include "flatbuffers/flatbuffers.h"
#include "lab_serial/assets_generated.h"

namespace li
{
	class TextureAtlasLoader
	{
	public:
		static Ref<TextureAtlas> Load(const Assets::TextureAtlas* atlas);
	};
}
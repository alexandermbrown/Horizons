#pragma once

#include "Lithium/Renderer/Font.h"

#include "flatbuffers/flatbuffers.h"
#include "lab_serial/assets_generated.h"

namespace li
{
	class FontLoader
	{
	public:
		static Ref<Font> Load(const Assets::Font* font);
	};
}
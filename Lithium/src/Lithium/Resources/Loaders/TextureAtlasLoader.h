#pragma once

#include "Lithium/Renderer/TextureAtlas.h"

#include "flatbuffers/flatbuffers.h"
#include "lab_serial/assets_generated.h"

namespace Li::Loaders
{
	Ref<Li::TextureAtlas> LoadTextureAtlas(const Assets::TextureAtlas* atlas);
}

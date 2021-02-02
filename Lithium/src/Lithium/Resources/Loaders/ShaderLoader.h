#pragma once

#include "Lithium/Core/Memory.h"
#include "Lithium/Renderer/Shader.h"

#include "flatbuffers/flatbuffers.h"
#include "lab_serial/assets_generated.h"

namespace Li::Loaders
{
	Ref<Shader> LoadShader(const Assets::Shader* shader);
}

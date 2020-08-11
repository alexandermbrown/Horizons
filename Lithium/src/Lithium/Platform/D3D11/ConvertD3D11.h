#pragma once

#include "Lithium/Renderer/RendererEnums.h"
#include <d3d11.h>

namespace li
{
	class ConvertD3D11
	{
	public:
		static D3D11_USAGE BufferUsage(BufferUsage usage);
		static D3D11_TEXTURE_ADDRESS_MODE WrapType(WrapType type);
	};
}
#pragma once

#include "Lithium/Renderer/RendererEnums.h"
#include <d3d11.h>

namespace li
{
	namespace ConvertD3D11
	{
		D3D11_USAGE BufferUsage(li::BufferUsage usage);
		D3D11_TEXTURE_ADDRESS_MODE WrapType(li::WrapType type);
		DXGI_FORMAT TextureFormat(int num_channels);
	};
}
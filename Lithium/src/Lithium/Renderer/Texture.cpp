#include "lipch.h"
#include "Texture.h"

#include "Lithium/Core/Application.h"

#ifdef LI_INCLUDE_OPENGL
#include "Lithium/Platform/OpenGL/OpenGLTexture.h"
#endif

#ifdef LI_INCLUDE_D3D11
#include "Lithium/Platform/D3D11/D3D11Texture.h"
#endif

namespace li
{
	Ref<Texture2D> Texture2D::Create(int width, int height, int channels, void* data,
		WrapType wrap_s, WrapType wrap_t, FilterType min_filter, FilterType mag_filter, bool dynamic)
	{
		switch (Application::Get()->GetAPI())
		{
#ifdef LI_INCLUDE_OPENGL
		case RendererAPI::OpenGL:
			return CreateRef<OpenGLTexture2D>(width, height, channels, data, wrap_s, wrap_t, min_filter, mag_filter);
#endif
#ifdef LI_INCLUDE_D3D11
		case RendererAPI::D3D11:
			return CreateRef<D3D11Texture2D>(width, height, channels, data, wrap_s, wrap_t, min_filter, mag_filter, dynamic, false);
#endif
		}

		LI_CORE_ERROR("Unknown RendererAPI!");
		return nullptr;
	}

	Ref<Texture2D> Texture2D::Create(const std::string& path, int desired_channels,
		WrapType wrap_s, WrapType wrap_t,
		FilterType min_filter, FilterType mag_filter)
	{
		switch (Application::Get()->GetAPI())
		{
#ifdef LI_INCLUDE_OPENGL
		case RendererAPI::OpenGL:
			return CreateRef<OpenGLTexture2D>(path, desired_channels, wrap_s, wrap_t, min_filter, mag_filter);
#endif
#ifdef LI_INCLUDE_D3D11
		case RendererAPI::D3D11:
			return CreateRef<D3D11Texture2D>(path, desired_channels, wrap_s, wrap_t, min_filter, mag_filter);
#endif
		}

		LI_CORE_ERROR("Unknown RendererAPI!");
		return nullptr;
	}

	Ref<Texture2D> Texture2D::Create(size_t image_size, const uint8_t* encoded_data, int desired_channels,
		WrapType wrap_s, WrapType wrap_t, FilterType min_filter, FilterType mag_filter)
	{
		switch (Application::Get()->GetAPI())
		{
#ifdef LI_INCLUDE_OPENGL
		case RendererAPI::OpenGL:
			return CreateRef<OpenGLTexture2D>(image_size, encoded_data, desired_channels, wrap_s, wrap_t, min_filter, mag_filter);
#endif
#ifdef LI_INCLUDE_D3D11
		case RendererAPI::D3D11:
			return CreateRef<D3D11Texture2D>(image_size, encoded_data, desired_channels, wrap_s, wrap_t, min_filter, mag_filter);
#endif
		}

		LI_CORE_ERROR("Unknown RendererAPI!");
		return nullptr;
	}
}
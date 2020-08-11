#include "lipch.h"
#include "Texture.h"

#include "Lithium/Renderer/Renderer.h"
#include "Lithium/Renderer/RendererAPI.h"

#ifdef LI_INCLUDE_OPENGL
#include "Lithium/Platform/OpenGL/OpenGLTexture.h"
#endif

#ifdef LI_INCLUDE_D3D11
#include "Lithium/Platform/D3D11/D3D11Texture.h"
#endif

namespace li
{
	Ref<Texture2D> Texture2D::Create(int width, int height, void* data,
		WrapType wrapS, WrapType wrapT,
		FilterType minFilter, FilterType magFilter)
	{
		switch (Renderer::GetAPI())
		{
#ifdef LI_INCLUDE_OPENGL
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLTexture2D>(width, height, data, wrapS, wrapT, minFilter, magFilter);
#endif
#ifdef LI_INCLUDE_D3D11
		case RendererAPI::API::D3D11:
			return CreateRef<D3D11Texture2D>(width, height, data, wrapS, wrapT, minFilter, magFilter);
#endif
		}

		LI_CORE_ERROR("Unknown RendererAPI!");
		return nullptr;
	}

	Ref<Texture2D> Texture2D::Create(const std::string& path, 
		WrapType wrapS, WrapType wrapT,
		FilterType minFilter, FilterType magFilter)
	{
		switch (Renderer::GetAPI())
		{
#ifdef LI_INCLUDE_OPENGL
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLTexture2D>(path, wrapS, wrapT, minFilter, magFilter);
#endif
#ifdef LI_INCLUDE_D3D11
		case RendererAPI::API::D3D11:
			return CreateRef<D3D11Texture2D>(path, wrapS, wrapT, minFilter, magFilter);
#endif
		}

		LI_CORE_ERROR("Unknown RendererAPI!");
		return nullptr;
	}

	Ref<Texture2D> Texture2D::Create(size_t imageSize, const uint8_t* rawData, WrapType wrapS, WrapType wrapT, FilterType minFilter, FilterType magFilter)
	{
		switch (Renderer::GetAPI())
		{
#ifdef LI_INCLUDE_OPENGL
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLTexture2D>(imageSize, rawData, wrapS, wrapT, minFilter, magFilter);
#endif
#ifdef LI_INCLUDE_D3D11
		case RendererAPI::API::D3D11:
			return CreateRef<D3D11Texture2D>(imageSize, rawData, wrapS, wrapT, minFilter, magFilter);
#endif
		}

		LI_CORE_ERROR("Unknown RendererAPI!");
		return nullptr;
	}
}
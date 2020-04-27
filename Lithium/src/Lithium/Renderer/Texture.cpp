#include "lipch.h"
#include "Texture.h"

#include "Lithium/Renderer/Renderer.h"
#include "Lithium/Renderer/RendererAPI.h"
#include "Lithium/Platform/OpenGL/OpenGLTexture.h"

namespace li
{
	Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height, 
		WrapType wrapS, WrapType wrapT,
		FilterType minFilter, FilterType magFilter)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLTexture2D>(width, height, wrapS, wrapT, minFilter, magFilter);
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
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLTexture2D>(path, wrapS, wrapT, minFilter, magFilter);
		}

		LI_CORE_ERROR("Unknown RendererAPI!");
		return nullptr;
	}

	Ref<Texture2D> Texture2D::Create(size_t imageSize, uint8_t* rawData, WrapType wrapS, WrapType wrapT, FilterType minFilter, FilterType magFilter)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLTexture2D>(imageSize, rawData, wrapS, wrapT, minFilter, magFilter);
		}

		LI_CORE_ERROR("Unknown RendererAPI!");
		return nullptr;
	}
}
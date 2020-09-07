#pragma once

#include "Lithium/Core/Core.h"
#include "Lithium/Renderer/RendererEnums.h"

#include <string>

namespace li 
{
	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual int GetWidth() const = 0;
		virtual int GetHeight() const = 0;

		virtual void Resize(int width, int height) = 0;
		virtual void AttachToFramebuffer() const = 0;
		virtual void Bind(uint32_t slot = 0) const = 0;
		virtual void* GetInternalTexture() const = 0;
	};

	class Texture2D : public Texture
	{
	public:
		static Ref<Texture2D> Create(int width, int height, void* data,
			WrapType wrapS = WrapType::ClampToEdge, WrapType wrapT = WrapType::ClampToEdge,
			FilterType minFilter = FilterType::Linear, FilterType magFilter = FilterType::Nearest, int channels = 4);

		static Ref<Texture2D> Create(const std::string& path,
			WrapType wrapS = WrapType::ClampToEdge, WrapType wrapT = WrapType::ClampToEdge,
			FilterType minFilter = FilterType::Linear, FilterType magFilter = FilterType::Nearest);

		static Ref<Texture2D> Create(size_t imageSize, const uint8_t* rawData,
			WrapType wrapS = WrapType::ClampToEdge, WrapType wrapT = WrapType::ClampToEdge,
			FilterType minFilter = FilterType::Linear, FilterType magFilter = FilterType::Nearest);
	};

}
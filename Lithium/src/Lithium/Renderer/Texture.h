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

		virtual void Bind(uint32_t slot = 0) const = 0;
		virtual void Resize(int width, int height) = 0;
		virtual void SetData(const void* data, int width, int height, bool discard) = 0;
		virtual void AttachToFramebuffer() const = 0;

		virtual void* GetInternalTexture() const = 0;
	};

	class Texture2D : public Texture
	{
	public:
		static Ref<Texture2D> Create(int width, int height, int channels, void* data,
			WrapType wrap_s = WrapType::ClampToEdge, WrapType wrap_t = WrapType::ClampToEdge,
			FilterType min_filter = FilterType::Linear, FilterType mag_filter = FilterType::Nearest, bool dynamic = false);

		static Ref<Texture2D> Create(const std::string& path, int desired_channels,
			WrapType wrap_s = WrapType::ClampToEdge, WrapType wrap_t = WrapType::ClampToEdge,
			FilterType min_filter = FilterType::Linear, FilterType mag_filter = FilterType::Nearest);

		static Ref<Texture2D> Create(size_t image_size, const uint8_t* encoded_data, int desired_channels,
			WrapType wrap_s = WrapType::ClampToEdge, WrapType wrapT = WrapType::ClampToEdge,
			FilterType min_filter = FilterType::Linear, FilterType mag_filter = FilterType::Nearest);
	};
}

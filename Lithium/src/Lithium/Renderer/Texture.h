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

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual void SetData(void* data, uint32_t size) = 0;
		virtual void Resize(uint32_t width, uint32_t height) = 0;
		virtual void AttachToFramebuffer(
			FramebufferAttachment attachment = FramebufferAttachment::DepthStencilAttachment,
			FramebufferTarget target = FramebufferTarget::Framebuffer
		) const = 0;

		virtual void Bind(uint32_t slot = 0) const = 0;
	};

	class Texture2D : public Texture
	{
	public:
		static Ref<Texture2D> Create(uint32_t width, uint32_t height, 
			WrapType wrapS = WrapType::ClampToEdge, WrapType wrapT = WrapType::ClampToEdge,
			FilterType minFilter = FilterType::Linear, FilterType magFilter = FilterType::Nearest);

		static Ref<Texture2D> Create(const std::string& path,
			WrapType wrapS = WrapType::ClampToEdge, WrapType wrapT = WrapType::ClampToEdge,
			FilterType minFilter = FilterType::Linear, FilterType magFilter = FilterType::Nearest);

		static Ref<Texture2D> Create(size_t imageSize, uint8_t* rawData,
			WrapType wrapS = WrapType::ClampToEdge, WrapType wrapT = WrapType::ClampToEdge,
			FilterType minFilter = FilterType::Linear, FilterType magFilter = FilterType::Nearest);
	};

}
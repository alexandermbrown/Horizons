#pragma once

#include "Lithium/Core/Core.h"
#include "Lithium/Renderer/RendererEnums.h"

namespace li
{
	class Renderbuffer
	{
	public:

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetStorage(uint32_t width, uint32_t height, ImageFormat internalFormat) = 0;
		virtual void Resize(uint32_t width, uint32_t height) = 0;
		virtual void AttachToFramebuffer(
			FramebufferAttachment attachment = FramebufferAttachment::DepthStencilAttachment,
			FramebufferTarget target = FramebufferTarget::Framebuffer
		) const = 0;

		static Ref<Renderbuffer> Create();
	};
}
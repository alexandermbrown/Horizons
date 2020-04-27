#pragma once

#include "Texture.h"
#include "RendererEnums.h"
#include "Renderbuffer.h"

namespace li
{
	class Framebuffer
	{
	public:

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual void AttachTexture(
			Ref<Texture2D> texture,
			FramebufferAttachment attachment
		) = 0;

		virtual void AttachRenderbuffer(
			Ref<Renderbuffer> renderBuffer, 
			FramebufferAttachment attachment
		) = 0;

		virtual Ref<Texture2D> GetTexture() const = 0;
		virtual void Resize(uint32_t width, uint32_t height) = 0;
		virtual bool IsComplete() const = 0;

		static Ref<Framebuffer> Create();
	};

}
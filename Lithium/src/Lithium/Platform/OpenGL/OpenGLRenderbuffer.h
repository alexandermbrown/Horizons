#pragma once

#include "Lithium/Core/Core.h"
#include "Lithium/Renderer/RendererEnums.h"

namespace Li
{
	class OpenGLRenderbuffer
	{
	public:

		OpenGLRenderbuffer(uint32_t width, uint32_t height);
		virtual ~OpenGLRenderbuffer();

		virtual void Bind() const;
		virtual void Unbind() const;

		virtual void Resize(uint32_t width, uint32_t height);
		virtual void AttachToFramebuffer() const;

	private:
		uint32_t m_RendererID;
	};
}
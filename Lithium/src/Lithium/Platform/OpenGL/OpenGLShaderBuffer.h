#pragma once

#include "Lithium/Renderer/ShaderBuffer.h"

namespace Li
{
	class OpenGLShaderBuffer : public ShaderBuffer
	{
	public:
		OpenGLShaderBuffer(uint32_t size);
		virtual ~OpenGLShaderBuffer();

		virtual void Bind(uint32_t slot) const;
		virtual void SetSubData(uint32_t* data, uint32_t size, uint32_t offset, bool discard);

	private:
		uint32_t m_RendererID;
	};
}

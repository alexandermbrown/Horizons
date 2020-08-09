#pragma once

#include "Lithium/Core/Core.h"
#include "Lithium/Renderer/Shader.h"
#include "zstr/zstr.hpp"

#include "ResourceArgs.h"
#include "Lithium/Renderer/RendererAPI.h"

#ifdef LI_INCLUDE_OPENGL
#include "Lithium/Platform/OpenGL/OpenGLShader.h"
#endif

#ifdef LI_INCLUDE_D3D11
#include "Lithium/Platform/D3D11/D3D11Shader.h"
#endif

namespace li
{
	class ShaderArgs : public ResourceArgs
	{
	public:
		ShaderArgs(zstr::ifstream* inFile, size_t* pos);

		virtual ~ShaderArgs();

		inline Ref<Shader> Create()
		{
			switch (RendererAPI::GetAPI())
			{
#ifdef LI_INCLUDE_OPENGL
			case RendererAPI::API::OpenGL:
				return CreateRef<OpenGLShader>(m_Name, m_GlslData);
#endif
#ifdef LI_INCLUDE_D3D11
			case RendererAPI::API::D3D11:
				return CreateRef<D3D11Shader>(m_Name, m_HlslVsData, m_HlslVsSize, m_HlslPsData, m_HlslPsSize);
#endif
			default:
				LI_CORE_ERROR("Unknown renderer API!");
				break;
			}
		};

	private:
		char* m_GlslData;

		size_t m_HlslVsSize;
		char* m_HlslVsData;

		size_t m_HlslPsSize;
		char* m_HlslPsData;
	};
}
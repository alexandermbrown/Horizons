#include "lipch.h"
#include "UniformBuffer.h"

#include "Lithium/Core/Application.h"
#include "Buffer.h"

#ifdef LI_INCLUDE_OPENGL
#include "Lithium/Platform/OpenGL/OpenGLUniformBuffer.h"
#endif

#ifdef LI_INCLUDE_D3D11
#include "Lithium/Platform/D3D11/D3D11UniformBuffer.h"
#endif

namespace Li
{
	Ref<UniformBuffer> Li::UniformBuffer::Create(uint32_t slot, uint32_t size)
	{
		switch (Application::Get().GetAPI())
		{
#ifdef LI_INCLUDE_OPENGL
		case RendererAPI::OpenGL:
			return MakeRef<OpenGLUniformBuffer>(slot, size);
#endif
#ifdef LI_INCLUDE_D3D11
		case RendererAPI::D3D11:
			return MakeRef<D3D11UniformBuffer>(slot, size);
#endif
		}

		LI_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}

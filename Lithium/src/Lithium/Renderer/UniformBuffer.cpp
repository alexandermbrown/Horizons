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
	// Returns alignment as multiple of 4 bytes.
	static uint32_t GetOpenGLAlignment(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:   return 1;
		case ShaderDataType::Float2:  return 2;
		case ShaderDataType::Float3:  return 4;
		case ShaderDataType::Float4:  return 4;
		case ShaderDataType::Mat4:    return 4;
		case ShaderDataType::Int:     return 1;
		case ShaderDataType::Int2:    return 2;
		case ShaderDataType::Int3:    return 4;
		case ShaderDataType::Int4:    return 4;
		}

		LI_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 1;
	}

	// Returns base alignment as multiple of 4 bytes.
	static uint32_t GetOpenGLBaseAlignment(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:   return 1;
		case ShaderDataType::Float2:  return 2;
		case ShaderDataType::Float3:  return 4;
		case ShaderDataType::Float4:  return 4;
		case ShaderDataType::Mat4:    return 16;
		case ShaderDataType::Int:     return 1;
		case ShaderDataType::Int2:    return 2;
		case ShaderDataType::Int3:    return 4;
		case ShaderDataType::Int4:    return 4;
		}

		LI_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 1;
	}

	UniformBufferLayout::UniformBufferLayout(const std::initializer_list<UniformBufferElement>& elements)
		: m_Elements(elements), m_Size(0)
	{
		// Current offset in multiples of 4 bytes.
		uint32_t componentOffset = 0;

		switch (Application::Get().GetAPI())
		{
		case RendererAPI::OpenGL:
			// Conform to OpenGL's std140 layout.
			// https://www.khronos.org/registry/OpenGL/extensions/ARB/ARB_uniform_buffer_object.txt
			// - Variables consuming N basic machine units have a base alignment of N.
			//       ie. aligned to size N boundaries.
			// - A float has alignment on boundary of size 1.
			// - A vec2 has alignment on boundary of size 2.
			// - A vec3 has alignment on boundary of size 4.
			// - A vec4 has alignment on boundary of size 4.
			// - A mat4 is stored as 4 vectors, aligned on boundary of size 4.

			for (UniformBufferElement& element : m_Elements)
			{
				componentOffset += componentOffset % GetOpenGLAlignment(element.Type);
				element.Offset = componentOffset * 4;
				componentOffset += GetOpenGLBaseAlignment(element.Type);
			}
			m_Size = componentOffset * 4;

			break;
		case RendererAPI::D3D11:
		{
			// Conform to D3D11's HLSL packing rules.
			// https://docs.microsoft.com/en-us/windows/win32/direct3dhlsl/dx-graphics-hlsl-packing-rules
			// - Variables aligned to 4-byte boundaries.
			// - Variables cannot cross a 16-byte boundary.

			for (UniformBufferElement& element : m_Elements)
			{
				uint32_t size = ShaderDataTypeSize(element.Type) / sizeof(float);
				// Check if the variable overlaps on boundary size 4,
				// using the floor effect of integer division.
				if ((componentOffset + size) / 4 > componentOffset / 4 && componentOffset % 4 > 0)
				{
					// Set componentOffset to the next multiple of 4.
					componentOffset = ((componentOffset / 4) + 1) * 4;
				}
				element.Offset = componentOffset * 4;
				componentOffset += size;
			}
			m_Size = componentOffset * 4;
			int overflow = m_Size % 16;
			if (overflow)
				m_Size += 16 - overflow;
			break;
		}
		default:
			LI_CORE_ERROR("Unsupported graphics API!");
		}

		
	}

	Ref<UniformBuffer> Li::UniformBuffer::Create(const std::string& name, uint32_t bindingSlot, ShaderType shaderType, const UniformBufferLayout& layout)
	{
		switch (Application::Get().GetAPI())
		{
#ifdef LI_INCLUDE_OPENGL
		case RendererAPI::OpenGL:
			return MakeRef<OpenGLUniformBuffer>(name, bindingSlot, layout);
#endif
#ifdef LI_INCLUDE_D3D11
		case RendererAPI::D3D11:
			return MakeRef<D3D11UniformBuffer>(name, bindingSlot, shaderType, layout);
#endif
		}

		LI_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}

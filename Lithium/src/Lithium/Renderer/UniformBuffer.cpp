#include "lipch.h"
#include "UniformBuffer.h"

#include "RendererAPI.h"
#include "Buffer.h"

#include "Lithium/Platform/OpenGL/OpenGLUniformBuffer.h"	

namespace li
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
		return 0;
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
		return 0;
	}

	UniformBufferLayout::UniformBufferLayout(const std::initializer_list<UniformBufferElement>& elements)
		: m_Elements(elements), m_Size(0)
	{
		// Current offset in multiples of 4 bytes.
		uint32_t componentOffset = 0;

		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::OpenGL:
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

			break;
		case RendererAPI::API::D3D11:
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

			break;
		default:
			LI_CORE_ERROR("Unsupported graphics API!");
		}

		m_Size = componentOffset * 4;
	}

	Ref<UniformBuffer> li::UniformBuffer::Create(const std::string& name, uint32_t bindingSlot, const UniformBufferLayout& layout)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::OpenGL:  return CreateRef<OpenGLUniformBuffer>(name, bindingSlot, layout);
		}

		LI_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}

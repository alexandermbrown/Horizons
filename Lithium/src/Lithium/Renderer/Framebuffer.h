#pragma once

#include "Texture.h"
#include "RendererEnums.h"
#include "Lithium/Core/Memory.h"

#include "glm/glm.hpp"

namespace Li
{
	class Framebuffer
	{
	public:
		virtual ~Framebuffer() = default;

		virtual void Bind() const = 0;
		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() const = 0;

		virtual Ref<Texture2D> GetTexture() const = 0;
		virtual void Resize(int width, int height) = 0;
		virtual glm::ivec2 GetSize() const = 0;

		static Ref<Framebuffer> Create(int width, int height);
	};
}

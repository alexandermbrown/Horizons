#pragma once

#include "Lithium/Renderer/Texture.h"
#include "zstr/zstr.hpp"

#include "ResourceArgs.h"

namespace li
{
	class Texture2DArgs : public ResourceArgs
	{
	public:
		Texture2DArgs(zstr::ifstream* inFile, size_t* pos);

		virtual ~Texture2DArgs();

		inline Ref<Texture2D> Create()
		{
			return Texture2D::Create(m_Size, m_GlslData, m_WrapS, m_WrapT, m_MinFilter, m_MagFilter);
		};

	private:
		size_t m_Size;
		uint8_t* m_GlslData;
		li::WrapType m_WrapS;
		li::WrapType m_WrapT;
		li::FilterType m_MinFilter;
		li::FilterType m_MagFilter;
		
	};
}
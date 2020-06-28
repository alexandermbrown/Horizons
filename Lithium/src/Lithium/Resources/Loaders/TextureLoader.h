#pragma once

#include "Lithium/Renderer/Texture.h"
#include "zstr/zstr.hpp"

#include "ResourceArgs.h"

namespace li
{
	class Texture2DArgs : public ResourceArgs
	{
	public:
		static Texture2DArgs* Deserialize(zstr::ifstream* inFile, size_t* pos);

		Texture2DArgs(const std::string& name, size_t imageSize, uint8_t* rawData, WrapType wrapS, WrapType wrapT, FilterType minFilter, FilterType magFilter)
			: ResourceArgs(SegmentType::Texture2D, name), m_Size(imageSize), m_Data(rawData), 
			m_WrapS(wrapS), m_WrapT(wrapT), m_MinFilter(minFilter), m_MagFilter(magFilter) {}

		virtual ~Texture2DArgs();

		inline Ref<Texture2D> Create()
		{
			return Texture2D::Create(m_Size, m_Data, m_WrapS, m_WrapT, m_MinFilter, m_MagFilter);
		};

	private:
		size_t m_Size;
		uint8_t* m_Data;
		li::WrapType m_WrapS;
		li::WrapType m_WrapT;
		li::FilterType m_MinFilter;
		li::FilterType m_MagFilter;
		
	};
}
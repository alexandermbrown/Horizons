#pragma once

#include "Lithium/Renderer/Shader.h"
#include "zstr/zstr.hpp"

#include "ResourceArgs.h"

namespace li
{
	class ShaderArgs : public ResourceArgs
	{
	public:
		static ShaderArgs* Deserialize(zstr::ifstream* inFile, size_t* pos);

		ShaderArgs(const std::string& name, char* data)
			: ResourceArgs(SegmentType::Shader, name), m_Data(data) {}

		virtual ~ShaderArgs();

		inline Ref<Shader> Create()
		{
			return Shader::Create(m_Name, m_Data);
		};

	private:
		char* m_Data;
	};
}
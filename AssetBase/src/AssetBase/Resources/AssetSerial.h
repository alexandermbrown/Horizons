#pragma once

#include "lab_serial/assets_generated.h"

#include <filesystem>

namespace AssetBase
{
	class AssetSerial
	{
	public:
		AssetSerial(const std::filesystem::path& path, bool debugMode);

		inline uint8_t* GetBufferPointer() const { return m_Builder.GetBufferPointer(); }
		inline uint32_t GetBufferSize() const { return m_Builder.GetSize(); }

	private:
		flatbuffers::FlatBufferBuilder m_Builder;
	};

	std::ostream& operator<<(std::ostream& os, const AssetSerial& e);
}

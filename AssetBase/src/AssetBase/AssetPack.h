#pragma once

#include "lab_serial/assets_generated.h"

#include <filesystem>

class AssetPack
{
public:
	AssetPack(const std::filesystem::path& path, bool debug_mode);

	inline uint8_t* GetBufferPointer() const { return m_Builder.GetBufferPointer(); }
	inline uint32_t GetBufferSize() const { return m_Builder.GetSize(); }

private:
	flatbuffers::FlatBufferBuilder m_Builder;
};

std::ostream& operator<<(std::ostream& os, const AssetPack& e);

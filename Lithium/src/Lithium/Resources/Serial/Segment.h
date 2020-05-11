#pragma once

#include <stdint.h>
#include <vector>
#include <memory>
#include <iostream>

namespace li
{
	enum class SegmentType
	{
		Texture2D,
		Shader,
		TextureAtlas,
		Font,
		Audio
	};

	class Segment
	{
	public:
		SegmentType type;

		Segment(SegmentType t)
			: type(t) {}
		virtual ~Segment() = default;

		virtual size_t GetSize() = 0;
	};

#pragma pack()
	struct HeaderSerial
	{
		const uint32_t signature = 0x42414c2b;
		size_t fileSize;
		size_t infoTableOffset;

		inline static size_t GetSize() {
			return sizeof(signature) + sizeof(fileSize) + sizeof(infoTableOffset);
		}
	};

#pragma pack()
	struct SegmentInfoTableEntry
	{
		uint32_t id;
		SegmentType type;
		size_t offset;

		inline static size_t GetSize() {
			return sizeof(id) + sizeof(type) + sizeof(offset);
		}
	};

	class SegmentInfoTable
	{
	public:
		uint32_t numEntries;
		SegmentInfoTableEntry* table;

		SegmentInfoTable(const std::vector<std::shared_ptr<Segment>>& segments);
		virtual ~SegmentInfoTable();
	};
}
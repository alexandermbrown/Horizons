#include "pch.h"
#include "Segment.h"

namespace AssetBase
{
	Segment::Segment(SegmentType t)
		: type(t)
	{

	}

	SegmentInfoTable::SegmentInfoTable(const std::vector<std::shared_ptr<Segment>>& segments)
	{
		size_t offset = HeaderSerial::GetSize();
		numEntries = (uint32_t)segments.size();
		table = new SegmentInfoTableEntry[numEntries];
		for (uint32_t i = 0; i < segments.size(); i++) 
		{
			auto& segment = segments.at(i);
			table[i].id = i;
			table[i].offset = offset;
			table[i].type = segment->type;

			offset += segment->GetSize();
		}
	}

	SegmentInfoTable::~SegmentInfoTable()
	{
		delete[] table;
	}

	std::ostream& operator<<(std::ostream& os, const SegmentInfoTable& sit)
	{
		os.write((char*)&sit.numEntries, sizeof(sit.numEntries));

		// TODO: test.
		for (int i = 0; i < sit.numEntries; i++)
		{
			os.write((char*)&sit.table[i].id, sizeof(sit.table[i].id));
			os.write((char*)&sit.table[i].type, sizeof(sit.table[i].type));
			os.write((char*)&sit.table[i].offset, sizeof(sit.table[i].offset));
		}

		return os;
	}
}

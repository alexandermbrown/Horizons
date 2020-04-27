#include "lipch.h"
#include "Segment.h"

namespace li
{
	SegmentInfoTable::SegmentInfoTable(const std::vector<std::shared_ptr<Segment>>& segments)
	{
		size_t offset = HeaderSerial::GetSize();
		numEntries = segments.size();
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
}

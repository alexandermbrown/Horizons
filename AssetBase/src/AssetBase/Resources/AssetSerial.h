#pragma once

#include "Segment.h"
#include <string>
#include <vector>
#include <filesystem>

namespace AssetBase
{
	class AssetSerial
	{
	public:
		AssetSerial(const std::filesystem::path& path);
		virtual ~AssetSerial() = default;

		inline const HeaderSerial& GetHeaderSerial() const { return m_HeaderSerial; }
		inline const std::unique_ptr<SegmentInfoTable>& GetSegmentInfoTable() const { return m_SegmentInfoTable; }
		inline const std::vector<std::shared_ptr<Segment>> GetSegments() const { return m_Segments; }

	private:
		HeaderSerial m_HeaderSerial;
		std::vector<std::shared_ptr<Segment>> m_Segments;
		std::unique_ptr<SegmentInfoTable> m_SegmentInfoTable;
	};

	std::ostream& operator<<(std::ostream& os, const AssetSerial& e);
}
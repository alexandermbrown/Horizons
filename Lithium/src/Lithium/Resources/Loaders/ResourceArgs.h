#pragma once

#include "../Serial/Segment.h"

namespace li
{
	class ResourceArgs
	{
	public:
		ResourceArgs(SegmentType type, const std::string& name)
			: m_Type(type), m_Name(name) {}

		virtual ~ResourceArgs() = default;

		inline SegmentType GetType() { return m_Type; }
		const std::string& GetName() { return m_Name; }

	protected:
		SegmentType m_Type;
		std::string m_Name;
	};
}
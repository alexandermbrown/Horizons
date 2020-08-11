#pragma once

class Query {
public:
	Query(const std::string name, const std::string value);
	const std::string m_Name;
	const std::string m_Value;
};
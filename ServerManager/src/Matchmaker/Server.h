#pragma once

#include <string>
#include <array>

enum class Location {
	AUS,
	USA,
};

class Server {
public:
	Location m_location;
	std::string m_address;
	std::array<std::string, 64> m_players;
};
#pragma once

#include <vector>
#include "Server.h"

class Matchmaker {
public:
	Matchmaker();
	Server Match(Location location);
private:
	std::vector<Server> m_servers;
};
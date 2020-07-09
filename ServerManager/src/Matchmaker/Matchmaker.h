#pragma once

#include <vector>
#include "Server.h"

class Matchmaker {
public:
	Matchmaker();
	~Matchmaker();
	Server* Match(std::string username, std::string location);
private:
	std::vector<Server*> m_Servers;
};
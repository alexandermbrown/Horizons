#pragma once

#include <vector>
#include "Server.h"
#include "Player.h"

class Matchmaker {
public:
	Matchmaker();
	~Matchmaker();
	Server* Match(Player* player, std::string location);
	inline std::vector<Server*> CheckServers() { return m_Servers; } // For development, remove later
private:
	std::vector<Server*> m_Servers;
};
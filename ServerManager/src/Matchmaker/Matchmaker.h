#pragma once

#include <vector>
#include "Server.h"
#include "Player.h"

class Matchmaker {
public:
	Matchmaker();
	~Matchmaker();
	Server* Match(Player* player, std::string location);
	inline const std::vector<Server*>& CheckServers() { return m_Servers; } // dev, remove later
private:
	std::vector<Server*> m_Servers;
};
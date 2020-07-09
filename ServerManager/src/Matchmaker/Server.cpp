#include "pch.h"
#include "Server.h"
#include <Matchmaker\Player.h>

Server::Server(const std::string& location, const std::string& address)
	: m_Location(location), m_Address(address), m_Players(), MaxPlayers(64)
{
	m_Players.reserve(MaxPlayers);
}

void Server::AddPlayer(Player* player)
{
	m_Players.push_back(player);
}

Server::~Server()
{
	for (Player* player : m_Players)
	{
		delete player;
	}
}

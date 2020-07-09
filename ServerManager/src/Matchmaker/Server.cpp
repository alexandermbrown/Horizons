#include "pch.h"
#include "Server.h"

Server::Server(const std::string& location, const std::string& address)
	: m_Location(location), m_Address(address), m_Players(), m_MaxPlayers(64)
{
	m_Players.reserve(m_MaxPlayers);
}

void Server::AddPlayer(std::string player)
{
	m_Players.push_back(player);
}

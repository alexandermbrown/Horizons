#include "pch.h"

#include "Matchmaker.h"

Matchmaker::Matchmaker()
{
	Server* server1 = new Server("USA", "192.111");
	Server* server2 = new Server("SYD", "192.222");
	Server* server3 = new Server("USA", "192.333");

	m_Servers.push_back(server1);
	m_Servers.push_back(server2);
	m_Servers.push_back(server3);
}

Matchmaker::~Matchmaker()
{
	for (Server* server : m_Servers)
	{
		delete server;
	}
}

Server* Matchmaker::Match(std::string username, std::string location)
{
	for (Server* server : m_Servers)
	{
		if (server->GetLocation() == location && server->GetPlayerCount() < server->m_MaxPlayers) {
			server->AddPlayer(username);
			return server;
		}
	}
	return nullptr;
}
#include "pch.h"

#include "Matchmaker.h"

Matchmaker::Matchmaker()
{
	Server example1 = Server();
	example1.m_location = Location::USA;
	m_servers.push_back(example1);

	Server example2 = Server();
	example2.m_location = Location::AUS;
	m_servers.push_back(example2);
}

Server Matchmaker::Match(Location location)
{
	Server result;
	for (int i = 0; i < m_servers.size(); i++) {
		if (m_servers[i].m_location == location) {
			result = m_servers[i];
			break;
		}
	}

	return result;
}

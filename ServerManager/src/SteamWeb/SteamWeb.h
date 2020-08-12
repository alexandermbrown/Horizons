#pragma once

#include "Query.h"

class SteamWeb {
public:
	SteamWeb(const std::string key, const std::string app);
	int AuthenticateTicket(std::string ticket);
private:
	std::string BuildQuery(std::vector<Query> queries);
	const std::string m_Key;
	const std::string m_AppID;
	httplib::Client m_Client;
};
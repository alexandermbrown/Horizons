#include "pch.h"
#include "httplib/httplib.h"

#include "Query.h"
#include "SteamWeb.h"

SteamWeb::SteamWeb(const std::string key, const std::string app)
	: m_Key(key), m_AppID(app), m_Client("https://api.steampowered.com", 80)
{
}

int SteamWeb::AuthenticateTicket(std::string ticket)
{
	std::string query = BuildQuery({
		Query("key", m_Key),
		Query("appid", m_AppID),
		Query("ticket", ticket)
	});

	std::string endpoint = "/ISteamUserAuth/AuthenticateUserTicket/v1/" + query;

	auto res = m_Client.Get(endpoint.c_str());
	if (res && res->status == 200) {
		std::cout << res->body << std::endl;
		return 222222; // Replace with 64bit SteamID
	}
	else {
		return 111111;
	}
}

std::string SteamWeb::BuildQuery(std::vector<Query> queries)
{
	std::string builtQuery = "?";
	bool appended = false;

	for (Query query : queries)
	{
		if (appended) {
			builtQuery += "&";
		}
		else {
			appended = true;
		}
		builtQuery += query.m_Name + "=" + query.m_Value;
	}

	return builtQuery;
}

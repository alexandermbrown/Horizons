#include "pch.h"
#include "httplib/httplib.h"
#include <string>

#include "Manager.h"
#include "Matchmaker/Matchmaker.h"
#include "SteamWeb/SteamWeb.h"

int main()
{
    Matchmaker matchmaker = Matchmaker();

    httplib::Server svr;
    SteamWeb steam = SteamWeb("REDACTED", "1389330");

    svr.Post("/authenticate", [&](const httplib::Request& req, httplib::Response& res) {
        auto size = req.files.size();

        if (!req.has_file("ticket")) {
            res.set_content("E001", "text/plain"); //E001 - did not pass ticket
            return;
        }

        if (!req.has_file("location")) {
            res.set_content("E002", "text/plain"); //E002 - did not pass location
            return;
        }

        const std::string& ticket = req.get_file_value("ticket").content;
        const std::string& location = req.get_file_value("location").content;

        const int steamID = steam.AuthenticateTicket(ticket);

        Player* player = new Player(steamID);
        Server* server = matchmaker.Match(player, location);
        if (server)
        {
            std::cout << "Connecting " << player->m_SteamID << " to " << server->GetAddress() << " with player count " << server->GetPlayerCount() << std::endl;
            res.set_content("OK. ip: " + server->GetAddress(), "text/plain");
        }
        else
        {
            res.set_content("Could not find a match.", "text/plain");
        }
        });

    svr.Get("/check", [&](const httplib::Request& req, httplib::Response& res) {
        std::string toSend = "";

        for (Server* server : matchmaker.CheckServers())
        {
            toSend += server->GetAddress() + " (" + std::to_string(server->GetPlayerCount()) + "/" + std::to_string(server->MaxPlayers)+ ")\n";
            for (Player* player : server->CheckPlayers())
            {
                toSend += " -   " + std::to_string(player->m_SteamID) + "\n";
            }
        }

        res.set_content(toSend, "text/plain");
        });

    svr.listen("localhost", 3001);
}
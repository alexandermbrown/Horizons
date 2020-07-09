#include "pch.h"
#include "httplib/httplib.h"
#include <string>

#include "Manager.h"
#include "Matchmaker/Matchmaker.h"

int main()
{
    Matchmaker matchmaker = Matchmaker();

    httplib::Server svr;

    svr.Post("/authenticate", [&](const httplib::Request& req, httplib::Response& res) {
        auto size = req.files.size();

        if (!req.has_file("username") || !req.has_file("password")) {
            res.set_content("Error. Did not pass username or password.", "text/plain");
            return;
        }

        if (!req.has_file("location")) {
            res.set_content("Error. Did not pass a desired location.", "text/plain");
            return;
        }

        const std::string& username = req.get_file_value("username").content;
        const std::string& password = req.get_file_value("password").content;
        const std::string& location = req.get_file_value("location").content;

        Server* server = matchmaker.Match(username, location);
        if (server)
        {
            std::cout << "Connecting " << username << " to " << server->GetAddress() << " with player count " << server->GetPlayerCount() << std::endl;
            res.set_content("OK. ip: " + server->GetAddress(), "text/plain");
        }
        else
        {
            res.set_content("Could not find a match.", "text/plain");
        }
        });

    svr.listen("localhost", 3001);
}
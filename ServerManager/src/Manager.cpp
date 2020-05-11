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


        try
        {
            Location location = (Location)std::stoul(req.get_file_value("location").content);
            Server result = matchmaker.Match(location);
            std::cout << (uint32_t)result.m_location << std::endl;
            res.set_content("Yay good job.", "text/plain");
            return;

        }
        catch (std::invalid_argument const& e)
        {
            res.set_content("Error. Invalid location.", "text/plain");
            return;
        }
        catch (std::out_of_range const& e)
        {
            res.set_content("Error. Invalid location.", "text/plain");
            return;
        }
        });

    svr.listen("localhost", 3001);
}
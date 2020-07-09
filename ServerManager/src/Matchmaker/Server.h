#pragma once

#include <string>
#include <array>

class Server {
public:
    Server(const std::string& location, const std::string& address);
    inline const std::string& GetLocation() const { return m_Location; }
    inline const std::string& GetAddress() const { return m_Address; }
    inline int GetPlayerCount() const { return m_Players.size(); };
    void AddPlayer(std::string);
    int m_MaxPlayers;
private:
    std::string m_Location;
    std::string m_Address;
    std::vector<std::string> m_Players;
};
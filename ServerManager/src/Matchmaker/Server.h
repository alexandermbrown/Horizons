#pragma once

#include <string>
#include <array>
#include "Player.h"

class Server {
public:
    Server(const std::string& location, const std::string& address);
    ~Server();
    inline const std::string& GetLocation() const { return m_Location; }
    inline const std::string& GetAddress() const { return m_Address; }
    inline int GetPlayerCount() const { return m_Players.size(); };
    inline const std::vector<Player*>& CheckPlayers() { return m_Players; } // dev, remove later
    void AddPlayer(Player*);
    const int MaxPlayers;
private:
    std::string m_Location;
    std::string m_Address;
    std::vector<Player*> m_Players;
};
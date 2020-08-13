#pragma once

#include <string>
#include <array>

class Player {
public:
    Player(const int& steamid);
    const int m_SteamID;
private:
    bool m_Connected;
};
#pragma once

#include <string>
#include <array>

class Player {
public:
    Player(const std::string& usr);
    const std::string username;
private:
    bool m_Connected;
};
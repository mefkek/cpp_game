#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class Room
{
    public:
    enum RoomType
    {
        Empty = 0,
        Treasure,
        Heal,
        Trap,
        Trader
    };
    sf::Vector2<std::uint32_t> position;
    std::vector<sf::Vector2i> exits;
    RoomType type;
    Room(sf::Vector2u pos = {0, 0}, std::vector<sf::Vector2i> exits = {});
};

class Corridor : public Room
{
    public:
    bool vertical;
    bool intersection;
};
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
    std::vector<sf::Vector2i> exits;
    RoomType type;
    Room(Room::RoomType type = Room::RoomType::Empty, std::vector<sf::Vector2i> exits = {});
    virtual ~Room() = default;
};

class Corridor : public Room
{
    public:
    bool vertical;
    bool intersection;
    Corridor(bool is_vertical, Room::RoomType type = Room::RoomType::Empty, std::vector<sf::Vector2i> exits = {});
};
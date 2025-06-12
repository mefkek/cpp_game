#pragma once
#include <SFML/Graphics.hpp>
#include <set>

struct Vector2iLess
{
    bool operator()(const sf::Vector2i& a, const sf::Vector2i& b) const;
};

using Vector2iSet = std::set<sf::Vector2i, Vector2iLess>;

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
    Vector2iSet exits;
    RoomType type;
    Room(Room::RoomType type = Room::RoomType::Empty, Vector2iSet exits = {});
    virtual ~Room() = default;
};

class Corridor : public Room
{
    public:
    bool vertical;
    bool intersection;
    Corridor(bool is_vertical, Room::RoomType type = Room::RoomType::Empty, Vector2iSet exits = {});
};
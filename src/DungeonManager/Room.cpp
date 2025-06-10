#include "DungeonManager/Room.hpp"

bool Vector2iLess::operator()(const sf::Vector2i& a, const sf::Vector2i& b) const
{
    if(a.x != b.x)
    {
        return a.x < b.x;
    }
    
    return a.y < b.y;
}

Room::Room(Room::RoomType type, Vector2iSet exits) : type(type), exits(exits) {}

Corridor::Corridor(bool is_vertical, Room::RoomType type, Vector2iSet exits)
                    : vertical(is_vertical), Room(type, exits) {}
#include "DungeonManager/Room.hpp"

Room::Room(Room::RoomType type, std::vector<sf::Vector2i> exits) : type(type), exits(exits) {}

Corridor::Corridor(bool is_vertical, Room::RoomType type, std::vector<sf::Vector2i> exits)
                    : vertical(is_vertical), Room(type, exits) {}
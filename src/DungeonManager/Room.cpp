#include "DungeonManager/Room.hpp"

Room::Room(Room::RoomType type, std::vector<sf::Vector2i> exits) : type(type), exits(exits) {}

Corridor::Corridor(bool is_vertical, Room::RoomType type) : vertical(is_vertical),
                   Room(type, (is_vertical ? 
                               std::vector<sf::Vector2i>{{0, 1}, {0, -1}} : 
                               std::vector<sf::Vector2i>{{1, 0}, {-1, 0}})) {}
#include "DungeonManager/Room.hpp"

Room::Room(sf::Vector2u pos, std::vector<sf::Vector2i> exits) : position{pos}, exits(exits) {}
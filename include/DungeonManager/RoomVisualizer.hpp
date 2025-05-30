#pragma once
#include "Room.hpp"
#include "Tilemap/Tilemap.hpp"
#include "Nodes/Application.hpp"
#include <vector>

class RoomVisualizer
{
    private:
    TextureAtlas& tileset;
    std::vector<sf::Vector2i> room_tiles;
    std::shared_ptr<Tilemap> room_tilemap;
    std::shared_ptr<sf::Sprite> special_element;

    public:
    RoomVisualizer(TextureAtlas& tileset);
    void visualize(const Room::RoomType type, sf::Vector2f position = {0, 0}, sf::Vector2f scale = {1.0f, 1.0f});
};

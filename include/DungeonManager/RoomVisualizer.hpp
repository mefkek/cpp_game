#pragma once
#include "Chunk.hpp"
#include "Tilemap/Tilemap.hpp"
#include "Nodes/Application.hpp"
#include <vector>

class RoomVisualizer
{
    private:
    TextureAtlas& tileset;
    std::vector<sf::Vector2i> room_tiles;
    std::shared_ptr<Tilemap> room_tilemap;
    std::shared_ptr<sf::Sprite> player_sprite;

    public:
    RoomVisualizer(TextureAtlas& tileset);
    void visualize(std::shared_ptr<Chunk> chunk, unsigned int chunk_size, sf::Vector2i chunk_pos, sf::Vector2f scale = {1.0f, 1.0f});
    void move(sf::Vector2i diff);
};

#pragma once
#include "Chunk.hpp"
#include "Tilemap/Tilemap.hpp"
#include "Nodes/Application.hpp"
#include <vector>

class RoomVisualizer
{
    private:
    using chunk_p = std::pair<sf::Vector2i, std::shared_ptr<Chunk>>;
    TextureAtlas& tileset;
    std::vector<sf::Vector2i> room_tiles;
    std::shared_ptr<Tilemap> room_tilemap;
    std::shared_ptr<sf::Sprite> player_sprite;

    public:
    RoomVisualizer(TextureAtlas& tileset);
    void visualize(const std::array<chunk_p, 9>& chunks, unsigned int chunk_size, sf::Vector2i chunk_pos, sf::Vector2f scale = {1.0f, 1.0f});
    void move(sf::Vector2i diff);
};

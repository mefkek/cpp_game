#pragma once
#include "Chunk.hpp"
#include "Tilemap/Tilemap.hpp"
#include "Nodes/Application.hpp"
#include <vector>

class RoomVisualizer
{
private:
    using chunk_p = std::pair<sf::Vector2i, std::shared_ptr<Chunk>>;
    const TextureAtlas& tileset;
    std::vector<sf::Vector2i> map_tiles;
    std::array<sf::Vector2i, 5 * 2 * 8 * 2> room_tiles;
    std::array<sf::Vector2i, 7 * 14> corridor_tiles;
    std::shared_ptr<Tilemap> room_tilemap;
    std::shared_ptr<Tilemap> map_tilemap;
    std::shared_ptr<sf::Sprite> player_sprite;
    void display_minimap(const std::vector<chunk_p>& chunks, unsigned int chunk_size, sf::Vector2i chunk_pos, sf::Vector2f scale = {1.0f, 1.0f});
    void display_room(const std::shared_ptr<Chunk> current_chunk, sf::Vector2i chunk_pos);

public:
    RoomVisualizer(const TextureAtlas& tileset);
    void visualize(const std::vector<chunk_p>& chunks, unsigned int chunk_size, sf::Vector2i chunk_pos, sf::Vector2f scale = {1.0f, 1.0f});
    void move(sf::Vector2i diff, std::shared_ptr<Chunk> current_chunk, sf::Vector2i chunk_pos);
};
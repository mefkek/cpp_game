#pragma once
#include "Chunk.hpp"

class ChunkGenerator
{
    private:
    int chunk_size;
    sf::Vector2u dungeon_size;
    std::size_t dungeon_seed;

    public:
    ChunkGenerator(int chunk_size, sf::Vector2u dungeon_size, std::size_t dungeon_seed);
    std::shared_ptr<Chunk> operator()(sf::Vector2i position);
};
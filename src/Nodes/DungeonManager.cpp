#include "Nodes/DungeonManager.hpp"
#include <random>
#include <sstream>
#include <string>
#include <cmath>

void DungeonManager::initialize()
{
    this->dungeon_seed = 10;    //for debugging only
    this->dungeon_size = {4, 4};
    this->chunk_size = 10;
}

DungeonManager::Chunk DungeonManager::get_chunk(sf::Vector2<std::int64_t> position)
{
    if(abs(position.x) > dungeon_size.x / 2 || abs(position.y) > dungeon_size.y / 2)
    {
        const std::string msg = "Position: " + std::to_string(position.x) +
                                ", " + std::to_string(position.y) + " is out of range";
        throw std::runtime_error(msg);
    }

    std::vector<sf::Vector2i> dirs = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};

    Chunk new_chunk;

    std::stringstream exit_hash;
    for(int i = 0; i < 4; ++i)
    {
        std::int64_t n_x = position.x + dirs[i].x;
        std::int64_t n_y = position.y + dirs[i].y;

        if(n_x >= 0 && n_x < chunk_size && n_y >= 0 && n_y < chunk_size)
        {
            if(position.x <= n_x)
            {
                exit_hash << "Chunk1_" << position.x << "_" << position.y
                          << "Chunk2_" << n_x << "_" << n_y
                          << "_Seed_" << dungeon_seed;
            }
            else
            {
                exit_hash << "Chunk1_" << n_x << "_" << n_y
                          << "Chunk2_" << position.x << "_" << position.y
                          << "_Seed_" << dungeon_seed;
            }

            std::size_t exit_seed = std::hash<std::string>{}(exit_hash.str());
            std::mt19937 gen(exit_seed);
            std::uniform_int_distribution<std::uint32_t> dist(0, chunk_size);
            new_chunk.exits[i] = {dirs[i], dist(gen)};
        }
    }
    

    std::stringstream to_hash;
    to_hash << "Chunk_" << position.x << "_" << position.y << "_Seed_" << dungeon_seed;
    std::size_t chunk_seed = std::hash<std::string>{}(to_hash.str());
}
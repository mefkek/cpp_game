#include "DungeonManager/ChunkGenerator.hpp"
#include "DungeonManager/Room.hpp"
#include "DungeonManager/Utils/ChunkGeneratorUtils.hpp"
#include "DungeonManager/Utils/DungeonRect.hpp"
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <queue>
#include <random>
#include <sstream>
#include <cmath>
#include <limits>

//constexpr int dungeon_min = 6u; included somewhere else

ChunkGenerator::ChunkGenerator(int chunk_size, sf::Vector2u dungeon_size, std::size_t dungeon_seed)
                               : chunk_size(chunk_size), dungeon_size(dungeon_size), dungeon_seed(dungeon_seed) {}

/// @brief BSP style chunk generator
/// @param position position of the chunk in a dungeon
/// @return The chunk at given position
/// @retval std::shared_ptr<Chunk> n_chunk
std::shared_ptr<Chunk> ChunkGenerator::operator()(sf::Vector2i position)
{
    //check if position is valid
    if(abs(position.x) > dungeon_size.x || abs(position.y) > dungeon_size.y)
    {
        const std::string msg = "Chunk at position: " + std::to_string(position.x) + ", " +
                                std::to_string(position.y) + " is out of range";
        return nullptr;
    }

    std::shared_ptr<Chunk> n_chunk = std::make_shared<Chunk>();
    std::vector<sf::Vector2i> chunk_exits;
    
    //generate 4 exits
    generate_exits(position, dungeon_size, dungeon_seed, chunk_size, chunk_exits, n_chunk);

    //get chunk hash and random device
    std::stringstream chunk_hash;
    chunk_hash << "Chunk_x" << position.x << "_y_" << position.y << "_Seed_" << dungeon_seed << "_"
               << "DungeonSize_" << dungeon_size.x << "_" << dungeon_size.y << "__";
    std::size_t chunk_seed = std::hash<std::string>{}(chunk_hash.str());
    std::mt19937 rd_dev(chunk_seed);

    generate_internal(n_chunk, rd_dev, chunk_size);

    std::queue<sf::Vector2i> q;
    for(int i = 0; i < chunk_exits.size(); ++i)
    {
        q.push(chunk_exits[i]);
    }

    int max_attempts = q.size() * 2;
    int attempts = 0;
    while(!q.empty())
    {
        attempts++;
        auto& e_pos = q.front();
        sf::Vector2i r_pos = {-1, -1};

        r_pos = find_closest(e_pos, n_chunk, chunk_size, chunk_exits, attempts > max_attempts);
        if(r_pos == sf::Vector2i{-1, -1})
        {
            q.push(e_pos);
        }
        else
        {
            connect_exit(e_pos, r_pos, n_chunk, chunk_size);
        }
        q.pop();
    }

    return n_chunk;
}
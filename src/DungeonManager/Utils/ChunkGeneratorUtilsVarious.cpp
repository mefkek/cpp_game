#include "DungeonManager/Utils/ChunkGeneratorUtils.hpp"
#include "DungeonManager/Utils/DungeonRect.hpp"
#include <algorithm>
#include <random>
#include <sstream>
#include <limits>

int sign(int v)
{
    if (v > 0)
    {
        return 1;
    }
    else if (v < 0)
    {
        return -1;
    }

    return 0;
}

std::string get_edge_hash(sf::Vector2i a, sf::Vector2i b, std::int64_t dungeon_seed,sf::Vector2u dungeon_size)
{
    if (a.x > b.x || (a.x == b.x && a.y > b.y))
    {
        std::swap(a, b);
    }

    std::stringstream ss;
    ss << "Chunk1_" << a.x << "_" << a.y << "Chunk2_" << b.x << "_" << b.y;
    ss << "_Seed_" << dungeon_seed << "_DungeonSize_" << dungeon_size.x;
    ss << "_" << dungeon_size.y;
    return ss.str();
}

bool check_surroundings(sf::Vector2i pos, std::shared_ptr<Chunk> n_chunk, int separation)
{
    for(int i = -separation; i < separation; ++i)
    {
        for(int j = -separation; j < separation; ++j)
        {
            if(i == 0 && j == 0)
            {
                continue;
            }

            sf::Vector2i check_pos = {pos.x + i, pos.y + j};
            if(n_chunk->rooms.find(check_pos.x) != n_chunk->rooms.end())
            {
                if(n_chunk->rooms[check_pos.x].find(check_pos.y) != n_chunk->rooms[check_pos.x].end())
                {
                    if(!std::dynamic_pointer_cast<Corridor>(n_chunk->rooms[check_pos.x][check_pos.y]))
                    {
                        return false;
                    }
                }
            }
        }
    }
    return true;    //position is valid
}

sf::Vector2i find_closest(sf::Vector2i exit_pos, std::shared_ptr<Chunk> n_chunk, int chunk_size, const std::vector<sf::Vector2i>& o_exits, bool fallback)
{
    sf::Vector2i res = {-1, -1};
    int separation = get_min_separation(chunk_size);
    int c_dist = std::numeric_limits<int>::max();
    bool is_horizontal = exit_pos.x == 0 || exit_pos.y == chunk_size - 1;

    for(const auto& [px, row] : n_chunk->rooms)
    {
        for(const auto& [py, room]: row)
        {
            if(px == exit_pos.x && py == exit_pos.y)
            {
                continue;
            }
            if(std::find(o_exits.begin(), o_exits.end(), sf::Vector2i{px,py}) != o_exits.end())
            {
                continue;
            }

            int dist = std::abs(exit_pos.x - px) + std::abs(exit_pos.y - py);
            if(dist < c_dist)
            {
                sf::Vector2i split_point;
                if(is_horizontal)
                {
                    split_point = {px, exit_pos.y};
                }
                else
                {
                    split_point = {exit_pos.x, py};
                }

                if(fallback)
                {
                    res = {px, py};
                    c_dist = dist;
                }
                else if(check_surroundings(split_point, n_chunk, separation))
                {
                    res = {px, py};
                    c_dist = dist;
                }
            }
        }
    }

    return res;
}
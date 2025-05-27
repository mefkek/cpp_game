#include "DungeonManager/Utils/ChunkGeneratorUtils.hpp"
#include <random>
#include <sstream>
#include <limits>
#include <algorithm>
#include <cmath>
#include <array>

constexpr int dungeon_min = 6u; //temporary

std::string get_edge_hash(sf::Vector2i a, sf::Vector2i b, std::int64_t dungeon_seed,
                          sf::Vector2u dungeon_size)
{
    if (a.x > b.x || (a.x == b.x && a.y > b.y)) {
        std::swap(a, b);
    }

    std::stringstream ss;
    ss << "Chunk1_" << a.x << "_" << a.y << "Chunk2_" << b.x << "_" << b.y;
    ss << "_Seed_" << dungeon_seed << "_DungeonSize_" << dungeon_size.x;
    ss << "_" << dungeon_size.y;
    return ss.str();
}

void generate_exits(sf::Vector2i position, sf::Vector2u dungeon_size, std::size_t dungeon_seed, int chunk_size, 
                    std::vector<std::tuple<sf::Vector2i, sf::Vector2i, int>>& exit_closest,
                    std::shared_ptr<Chunk> n_chunk)
{
    std::array<sf::Vector2i, 4> dirs = {sf::Vector2i(1, 0), {0, 1}, {-1, 0}, {0, -1}};
    for (int i = 0; i < 4; ++i)
    {
        sf::Vector2i adjacent = {position.x + dirs[i].x, position.y + dirs[i].y};
        if (abs(adjacent.x) > dungeon_size.x || abs(adjacent.y) > dungeon_size.y)
        {
            // skip if no exit
            continue;
        }

        // get exit random device
        std::size_t exit_seed = std::hash<std::string>{}(get_edge_hash(position, adjacent, dungeon_seed, dungeon_size));
        std::mt19937 rd_dev(exit_seed);
        std::uniform_int_distribution<std::uint32_t> dist(std::min(dungeon_min, 2 * (chunk_size / 5)),
                                                          chunk_size - std::min(dungeon_min, 2 * (chunk_size / 5)));
        int exit_pos = dist(rd_dev);
        sf::Vector2i n_pos;
        sf::Vector2i exit;

        if (dirs[i].x != 0)
        {
            n_pos.x = (dirs[i].x > 0) ? chunk_size - 1 : 0;
            n_pos.y = exit_pos;
            exit_closest.push_back({n_pos, {0, 0}, std::numeric_limits<int>().max()});
            exit = {(n_pos.x == 0 ? 1 : -1), 0};
        }
        else if (dirs[i].y != 0)
        {
            n_pos.x = exit_pos;
            n_pos.y = (dirs[i].y > 0) ? chunk_size - 1 : 0;
            exit_closest.push_back({n_pos, {0, 0}, std::numeric_limits<int>().max()});
            exit = {0, (n_pos.y == 0 ? 1 : -1)};
        }

        if (n_pos.x == 0 || n_pos.y == 0)
        {
            bool vertical = n_pos.y == 0;
            n_chunk->rooms[n_pos.x][n_pos.y] = std::make_shared<Corridor>(vertical, Room::RoomType::Empty, std::vector{exit});
        }
        else
        {
            n_chunk->rooms[n_pos.x][n_pos.y] = std::make_shared<Room>(Room::RoomType::Empty, std::vector{exit});
        }
    }
}

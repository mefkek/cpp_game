#include "DungeonManager/Utils/ChunkGeneratorUtils.hpp"
#include "DungeonManager/Utils/DungeonRect.hpp"
#include <random>
#include <sstream>
#include <limits>
#include <algorithm>
#include <cmath>
#include <array>
#include <queue>
#include <tuple>

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

std::string get_edge_hash(sf::Vector2i a, sf::Vector2i b, std::int64_t dungeon_seed,
                          sf::Vector2u dungeon_size)
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

void generate_exits(sf::Vector2i position, sf::Vector2u dungeon_size, std::size_t dungeon_seed, int chunk_size,
                    std::vector<std::tuple<sf::Vector2i, sf::Vector2i, int>> &exit_closest,
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

void generate_internal(std::shared_ptr<Chunk> n_chunk, std::mt19937 &rd_dev, int chunk_size)
{
    // prapare queue for room generation
    std::bernoulli_distribution bern(0.5);
    std::queue<std::tuple<std::shared_ptr<DungeonRect>, bool>> q;
    std::shared_ptr<DungeonRect> root = std::make_shared<DungeonRect>();
    root->rect = sf::IntRect({std::min(dungeon_min, 2 * (chunk_size / 5)), std::min(dungeon_min, 2 * (chunk_size / 5))},
                             {chunk_size - std::min(dungeon_min, 2 * (chunk_size / 5)),
                              chunk_size - std::min(dungeon_min, 2 * (chunk_size / 5))}); // change into a constant later
    q.push({root, false});
    q.push({root, false});

    while (!q.empty())
    {
        auto [d, visited] = q.front();
        q.pop();

        if (!visited)
        {
            if (!d->left && !d->right)
            {
                // divide if not visited
                d->divide(rd_dev, (bern(rd_dev) > 0.5 ? sf::Vector2i(1, 0) : sf::Vector2i(0, 1)), chunk_size);
            }
            if (d->left)
            {
                q.push({d->left, false});
            }
            if (d->right)
            {
                q.push({d->right, false});
            }
            if (!d->left && !d->right)
            {
                sf::Vector2i n_room_pos = d->rect.getCenter();
                if (auto &c_ptr = n_chunk->rooms[n_room_pos.x][n_room_pos.y])
                {
                    std::vector<sf::Vector2i> o_exits = c_ptr->exits;
                    c_ptr = std::make_shared<Room>(Room::RoomType::Empty, o_exits);
                }
                else
                {
                    c_ptr = std::make_shared<Room>();
                }
            }
        }
        else
        {
            if (d->left && d->right)
            {
                // connect rooms / rectangles
                sf::Vector2i l = d->left->rect.getCenter();
                sf::Vector2i r = d->right->rect.getCenter();
                sf::Vector2i dir = {sign(r.x - l.x), sign(r.y - l.y)};
                sf::Vector2i pos = l;

                while (pos != r)
                {
                    sf::Vector2i n_pos = pos + dir;
                    std::shared_ptr<Room> &tile = n_chunk->rooms[n_pos.x][n_pos.y];

                    if (!tile)
                    {
                        bool is_vertical = dir.y != 0;
                        tile = std::make_shared<Corridor>(is_vertical);
                        tile->exits.push_back(-dir);
                    }
                    else if (tile)
                    {
                        tile->exits.push_back(-dir);
                    }

                    if (auto &c_ptr = n_chunk->rooms[pos.x][pos.y])
                    {
                        c_ptr->exits.push_back(dir);
                    }
                    else
                    {
                        c_ptr = std::make_shared<Room>(Room::RoomType::Empty, std::vector{dir});
                    }

                    pos = n_pos;
                }

                if (auto &r_ptr = n_chunk->rooms[pos.x][pos.y])
                {
                    r_ptr->exits.push_back(-dir);
                }
                else
                {
                    r_ptr = std::make_shared<Room>(Room::RoomType::Empty, std::vector{-dir});
                }
            }

            continue;
        }

        visited = true;
        q.push({d, visited});
    }
}
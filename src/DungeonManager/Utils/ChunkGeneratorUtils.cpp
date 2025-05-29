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
                    std::vector<sf::Vector2i>& exits,
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
        std::uniform_int_distribution<std::uint32_t> dist(get_min_separation(chunk_size),
                                                          chunk_size - 2 * get_min_separation(chunk_size));
        int exit_pos = dist(rd_dev);
        sf::Vector2i n_pos;
        sf::Vector2i exit;

        if (dirs[i].x != 0)
        {
            n_pos.x = (dirs[i].x > 0) ? chunk_size - 1 : 0;
            n_pos.y = exit_pos;
            exits.push_back(n_pos);
            exit = {(n_pos.x == 0 ? 1 : -1), 0};
        }
        else if (dirs[i].y != 0)
        {
            n_pos.x = exit_pos;
            n_pos.y = (dirs[i].y > 0) ? chunk_size - 1 : 0;
            exits.push_back(n_pos);
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
    std::bernoulli_distribution bern(0.5);

    // prapare queue for room generation
    std::queue<std::tuple<std::shared_ptr<DungeonRect>, bool>> q;
    std::shared_ptr<DungeonRect> root = std::make_shared<DungeonRect>();

    sf::Vector2i d_rect_start_pos = {get_min_separation(chunk_size), get_min_separation(chunk_size)};
    sf::Vector2i d_rect_size = {chunk_size - 2 * get_min_separation(chunk_size),
                                chunk_size - 2 * get_min_separation(chunk_size)};

    root->rect = sf::IntRect(d_rect_start_pos, d_rect_size);

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
                bool is_vertical = (dir.y != 0);

                if(auto& l_ptr = n_chunk->rooms[l.x][l.y])
                {
                    l_ptr->exits.push_back(dir);
                }
                else
                {
                    l_ptr = std::make_shared<Room>();
                    l_ptr->exits.push_back(dir);
                }

                pos += dir;
                while (pos != r)
                {
                    std::vector<sf::Vector2i> o_exits;
                    auto& p_ptr= n_chunk->rooms[pos.x][pos.y];
                    if(p_ptr)
                    {
                        o_exits = p_ptr->exits;
                    }

                    p_ptr = std::make_shared<Corridor>(is_vertical, Room::RoomType::Empty, std::vector{-dir, dir});

                    for(auto& ex : o_exits)
                    {
                        p_ptr->exits.push_back(ex);
                    }

                    pos += dir;
                }

                if(auto& r_ptr = n_chunk->rooms[r.x][r.y])
                {
                    r_ptr->exits.push_back(-dir);
                }
                else
                {
                    r_ptr = std::make_shared<Room>();
                    r_ptr->exits.push_back(-dir);
                }
            }
            continue;
        }

        visited = true;
        q.push({d, visited});
    }
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

void connect_exit(sf::Vector2i exit_pos, sf::Vector2i goal, std::shared_ptr<Chunk> n_chunk, int chunk_size)
{
    bool is_vertical = exit_pos.y == 0 || exit_pos.y == chunk_size - 1;
    int dx = goal.x - exit_pos.x;
    int dy = goal.y - exit_pos.y;
    sf::Vector2i diff1 = {sign(dx), 0}, diff2 = {0, sign(dy)};
    sf::Vector2i diff = diff1;

    if(is_vertical)
    {
        diff = diff2;
        diff2 = diff1;
    }
    else
    {
        diff = diff1;
    }

    sf::Vector2i pos = exit_pos + diff;
    while(true)
    {
        auto& tile = n_chunk->rooms[pos.x][pos.y];
        if(tile)
        {
            tile->exits.push_back(-diff);
            return;
        }
        tile = std::make_shared<Corridor>(is_vertical);
        tile->exits.push_back(-diff);

        bool split_reached = (is_vertical) ? pos.y == goal.y : pos.x == goal.x;
        if(split_reached)
        {
            if(diff == diff2)
            {
                return;
            }

            diff = diff2;
            is_vertical = !is_vertical;

            auto old_exits = tile->exits;
            tile = std::make_shared<Room>();
            tile->exits = old_exits;
        }
        
        tile->exits.push_back(diff);
        pos += diff;
    }
}

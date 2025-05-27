#include "DungeonManager/ChunkGenerator.hpp"
#include "DungeonManager/Room.hpp"
#include "DungeonManager/Utils/ChunkGeneratorUtils.hpp"
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <queue>
#include <random>
#include <sstream>
#include <cmath>
#include <limits>

constexpr int dungeon_min = 6u;

ChunkGenerator::ChunkGenerator(unsigned int chunk_size, sf::Vector2u dungeon_size, std::size_t dungeon_seed)
                               : chunk_size(chunk_size), dungeon_size(dungeon_size), dungeon_seed(dungeon_seed) {}

struct DungeonRect
{
    sf::IntRect rect;
    std::shared_ptr<DungeonRect> left;
    std::shared_ptr<DungeonRect> right;

    void divide(std::mt19937& rd_dev, sf::Vector2i division_axis, unsigned int chunk_size)
    {
        sf::Vector2i bounds = rect.size;
        sf::Vector2i offset = rect.position;

        unsigned int d_bound = (division_axis.x != 0) ? rect.size.x : rect.size.y;
        long long min_div_size = std::min(dungeon_min, 2 * (static_cast<int>(chunk_size) / 5));
        if(min_div_size % 2 != 0)
        {
            ++min_div_size;
        }

        std::uniform_int_distribution dist(min_div_size, d_bound - min_div_size);
        long long d_pos = dist(rd_dev);

        sf::IntRect l_rect;
        l_rect.position = offset;
        l_rect.size = (division_axis.x != 0) ? sf::Vector2i(d_pos, bounds.y) : sf::Vector2i(bounds.x, d_pos);

        sf::IntRect r_rect;
        r_rect.position = {offset.x + (l_rect.size.x * division_axis.x), offset.y + (l_rect.size.y * division_axis.y)};
        r_rect.size = {bounds.x - (l_rect.size.x * division_axis.x), bounds.y - (l_rect.size.y * division_axis.y)};

        if(l_rect.size.x >= min_div_size && l_rect.size.y >= min_div_size && r_rect.size.x >= min_div_size && r_rect.size.y >= min_div_size)
        {
            left = std::make_shared<DungeonRect>();
            left->rect = l_rect;

            right = std::make_shared<DungeonRect>();
            right->rect = r_rect;
        }
    }

    void divide_along(int split_pos, bool horizontal, unsigned int chunk_size)
    {
        sf::Vector2i bounds = rect.size;
        sf::Vector2i offset = rect.position;

        long long min_div_size = std::min(dungeon_min, 2 * (static_cast<int>(chunk_size) / 5));
        if(min_div_size % 2 != 0)
        {
            ++min_div_size;
        }

        if (horizontal)
        {
            // Global to local conversion
            int local_split = split_pos - offset.y;

            if (local_split <= min_div_size || bounds.y - local_split <= min_div_size)
            {
                return;
            }

            sf::IntRect l_rect{offset, sf::Vector2i(bounds.x, local_split)};
            sf::IntRect r_rect{sf::Vector2i(offset.x, offset.y + local_split),
                            sf::Vector2i(bounds.x, bounds.y - local_split)};

            left = std::make_shared<DungeonRect>();
            left->rect = l_rect;

            right = std::make_shared<DungeonRect>();
            right->rect = r_rect;
        }
        else
        {
            int local_split = split_pos - offset.x;

            if (local_split <= min_div_size || bounds.x - local_split <= min_div_size)
            {
                return;
            }

            sf::IntRect l_rect{offset, sf::Vector2i(local_split, bounds.y)};
            sf::IntRect r_rect{sf::Vector2i(offset.x + local_split, offset.y),
                           sf::Vector2i(bounds.x - local_split, bounds.y)};

            left = std::make_shared<DungeonRect>();
            left->rect = l_rect;

            right = std::make_shared<DungeonRect>();
            right->rect = r_rect;
        }
    }
};

/// @brief BSP style chunk generator
/// @param position position of the chunk in a dungeon
/// @return The chunk at given position
/// @retval std::shared_ptr<Chunk> n_chunk
std::shared_ptr<Chunk> ChunkGenerator::operator()(sf::Vector2i position)
{
    //helper for vector "normalization"
    auto sign = [](int v){if(v > 0) return 1; else if(v < 0) return -1; return 0;};

    //check if position is valid
    if(abs(position.x) > dungeon_size.x || abs(position.y) > dungeon_size.y)
    {
        const std::string msg = "Chunk at position: " + std::to_string(position.x) + ", " + std::to_string(position.y) +
                                " is out of range";
        throw std::runtime_error(msg);
    }

    std::shared_ptr<Chunk> n_chunk = std::make_shared<Chunk>();
    std::vector<std::tuple<sf::Vector2i, sf::Vector2i, int>> exit_closest;
    const int chunk_size_i = static_cast<int>(chunk_size);
    
    //generate 4 exits
    generate_exits(position, dungeon_size, dungeon_seed, chunk_size_i, exit_closest, n_chunk);

    //prapare queue for room generation
    std::queue<std::tuple<std::shared_ptr<DungeonRect>, bool>> q;
    std::shared_ptr<DungeonRect> root = std::make_shared<DungeonRect>();
    root->rect = sf::IntRect({std::min(dungeon_min, 2 * (chunk_size_i / 5)), std::min(dungeon_min, 2 * (chunk_size_i / 5))},
                             {chunk_size_i - std::min(dungeon_min, 2 * (chunk_size_i / 5)),
                             chunk_size_i - std::min(dungeon_min, 2 * (chunk_size_i / 5))});  //change into a constant later
    q.push({root, false});

    //get chunk hash and radnom device
    std::stringstream chunk_hash;
    chunk_hash << "Chunk_x" << position.x << "_y_" << position.y << "_Seed_" << dungeon_seed << "_"
               << "DungeonSize_" << dungeon_size.x << "_" << dungeon_size.y << "__";
    std::size_t chunk_seed = std::hash<std::string>{}(chunk_hash.str());
    std::mt19937 rd_dev(chunk_seed);
    std::bernoulli_distribution bern_dist(0.5);
    
    auto is_too_close= [&](sf::Vector2i pos)
    {
        int valid_range = std::min(dungeon_min, 2 * (chunk_size_i / 5));
        int half_range = valid_range/ 2;

        for (int dx = -half_range; dx <= half_range; ++dx)
        {
            for (int dy = -half_range; dy <= half_range; ++dy)
            {
                if (std::abs(dx) == std::abs(dy))
                {
                    continue;
                }

                sf::Vector2i n = pos + sf::Vector2i(dx, dy);

                if (n_chunk->rooms.count(n.x) && n_chunk->rooms[n.x].count(n.y))
                {
                    auto room = n_chunk->rooms[n.x][n.y];
                    if (!std::dynamic_pointer_cast<Corridor>(room))
                    {
                        return true;
                    }
                }
            }
        }
        return false;
    };

    auto check_if_closer = [&](sf::Vector2i& exit_pos, sf::Vector2i& current_c, int& c_dist, sf::Vector2i n_r, bool is_fallback = false)
    {
        if (exit_pos == n_r) return;

        bool is_exit = std::any_of(exit_closest.begin(), exit_closest.end(), [n_r](const std::tuple<sf::Vector2i, sf::Vector2i, int>& t)
                                  {
                                      const auto& [e, r, d] = t;
                                      return e == n_r;
                                  });
        if (is_exit && !is_fallback) return;

        if (is_too_close(n_r) && !is_fallback) return;

        int dx = std::abs(n_r.x - exit_pos.x);
        int dy = std::abs(n_r.y - exit_pos.y);

        bool is_vertical_exit = (exit_pos.x == 0 || exit_pos.x == chunk_size_i - 1);

        sf::Vector2i divergence_point = is_vertical_exit
            ? sf::Vector2i{ exit_pos.y, n_r.x }
            : sf::Vector2i{ n_r.y, exit_pos.x };

        if(divergence_point != exit_pos && is_too_close(divergence_point) && !is_fallback) return;


        int offset_from_axis = is_vertical_exit ? dy : dx;


        if (offset_from_axis != 0 && offset_from_axis < std::min(dungeon_min, 2 * (chunk_size_i / 5)) / 2  && !is_fallback)
        {
            return;
        }

        if (offset_from_axis < c_dist)
        {
            current_c = n_r;
            c_dist = offset_from_axis;
        }
    };

    while(!q.empty())
    {
        auto [d, visited] = q.front();
        q.pop();

        if(!visited)
        {
            if(!d->left && !d->right)
            {
                //divide if not visited
                d->divide(rd_dev, (bern_dist(rd_dev) > 0.5 ? sf::Vector2i(1, 0) : sf::Vector2i(0, 1)), chunk_size_i);
            }
            if(d->left)
            {
                q.push({d->left, false});
            }
            if(d->right)
            {
                q.push({d->right, false});
            }
            if(!d->left && !d->right)
            {
                sf::Vector2i n_room_pos = d->rect.getCenter();
                if(auto& c_ptr = n_chunk->rooms[n_room_pos.x][n_room_pos.y])
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
            if(d->left && d->right)
            {
                //connect rooms / rectangles
                sf::Vector2i l = d->left->rect.getCenter();
                sf::Vector2i r = d->right->rect.getCenter();
                sf::Vector2i dir = {sign(r.x - l.x), sign(r.y - l.y)};
                sf::Vector2i pos = l;

                while(pos != r)
                {
                    sf::Vector2i n_pos = pos + dir;
                    std::shared_ptr<Room>& tile = n_chunk->rooms[n_pos.x][n_pos.y];

                    if(!tile)
                    {
                        bool is_vertical = dir.y != 0;
                        tile = std::make_shared<Corridor>(is_vertical);
                        tile->exits.push_back(-dir);
                    }
                    else if(tile)
                    {
                        tile->exits.push_back(-dir);
                    }

                    if(auto& c_ptr = n_chunk->rooms[pos.x][pos.y])
                    {
                        c_ptr->exits.push_back(dir);
                    }
                    else
                    {
                        c_ptr = std::make_shared<Room>(Room::RoomType::Empty, std::vector{dir});
                    }

                    pos = n_pos;
                }

                if(auto& r_ptr = n_chunk->rooms[pos.x][pos.y])
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

    //rewrite this later
    for(int i = 0; i < exit_closest.size(); ++i)
    {
        {
            auto& [e, r, d] = exit_closest[i];
            for(auto [px, row] : n_chunk->rooms)
            {
                for(auto [py, room] : row)
                {
                    check_if_closer(e, r, d, {px, py});
                }
            }
            if(r == sf::Vector2i(0, 0))
            {
                for(auto [px, row] : n_chunk->rooms)
                {
                    for(auto [py, room] : row)
                    {
                        check_if_closer(e, r, d, {px, py}, true);
                    }
                }
            }
        }

        auto& [e_pos, r_pos, d] = exit_closest[i];
        int dx = r_pos.x - e_pos.x;
        int dy = r_pos.y - e_pos.y;
        sf::Vector2i diff1, diff2;
        int j_m, k_m;
        bool do_ver;
        bool end = false;
        if(e_pos.x == 0 || e_pos.x == chunk_size_i - 1)
        {
            do_ver = false;
            j_m = std::abs(dx);
            k_m = std::abs(dy) - 1;
            diff1.x = sign(dx);
            diff1.y = 0;
            diff2.x = 0;
            diff2.y = sign(dy);
        }
        else
        {
            do_ver = true;
            j_m = std::abs(dy);
            k_m = std::abs(dx) - 1;
            diff1.x = 0;
            diff1.y = sign(dy);
            diff2.x = sign(dx);
            diff2.y = 0;
        }

        std::shared_ptr<Room> room;
        sf::Vector2i pos = {e_pos.x, e_pos.y};
        std::shared_ptr<Room> last_placed = n_chunk->rooms[pos.x][pos.y];
        for(int j = 0; j < j_m; ++j)
        {
            pos += diff1;
            if(auto r = n_chunk->rooms[pos.x][pos.y])
            {
                r->exits.push_back(-diff1);
                last_placed->exits.push_back(-diff1);
                end = true;
                break;
            }
            last_placed->exits.push_back(-diff1);
            n_chunk->rooms[pos.x][pos.y] = std::make_shared<Corridor>(do_ver);
            last_placed = n_chunk->rooms[pos.x][pos.y];
            last_placed->exits.push_back(diff1);
        }

        if(end)
        {
            continue;
        }

        do_ver = !do_ver;
        if(last_placed == n_chunk->rooms[pos.x][pos.y])
        {
            std::vector<sf::Vector2i> n_exits;
            for(auto& ex : last_placed->exits)
            {
                n_exits.push_back(-ex);
                last_placed->exits.push_back(-diff2);
            }
            n_chunk->rooms[pos.x][pos.y] = std::make_shared<Room>(Room::RoomType::Empty, n_exits);
            n_chunk->rooms[pos.x][pos.y]->exits.push_back(diff2);
        }

        for(int k = 0; k < k_m; ++k)
        {
            pos += diff2;
            if(auto r = n_chunk->rooms[pos.x][pos.y])
            {
                r->exits.push_back(-diff2);
                break;
            }

            if(last_placed)
            {   
                last_placed->exits.push_back(-diff2);
            }
            n_chunk->rooms[pos.x][pos.y] = std::make_shared<Corridor>(do_ver);
            last_placed = n_chunk->rooms[pos.x][pos.y];
            last_placed->exits.push_back(diff2);
        }
    }

    return n_chunk;
}
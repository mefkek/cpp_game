#include "DungeonManager/ChunkGenerator.hpp"
#include "DungeonManager/Room.hpp"
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <queue>
#include <random>
#include <sstream>
#include <cmath>
#include <limits>

#include <iostream>

constexpr unsigned int dungeon_min = 6u;

ChunkGenerator::ChunkGenerator(unsigned int chunk_size, sf::Vector2u dungeon_size, std::size_t dungeon_seed)
                               : chunk_size(chunk_size), dungeon_size(dungeon_size), dungeon_seed(dungeon_seed) {}

//helper function
std::string get_edge_hash(sf::Vector2<std::int64_t> a, sf::Vector2<std::int64_t> b, std::int64_t dungeon_seed,
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
        long long min_div_size = std::min(dungeon_min, 2 * (chunk_size / 5));
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

        long long min_div_size = std::min(dungeon_min, 2 * (chunk_size / 5));
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
std::shared_ptr<Chunk> ChunkGenerator::operator()(sf::Vector2<std::int64_t> position)
{
    //check if position is valid
    if(abs(position.x) > dungeon_size.x || abs(position.y) > dungeon_size.y)
    {
        const std::string msg = "Position: " + std::to_string(position.x) + ", " + std::to_string(position.y) + " is out of range";
        throw std::runtime_error(msg);
    }

    std::array<sf::Vector2i, 4> dirs = {sf::Vector2i(1, 0), {0, 1}, {-1, 0}, {0, -1}};
    std::shared_ptr<Chunk> n_chunk = std::make_shared<Chunk>();
    std::vector<std::tuple<sf::Vector2u, sf::Vector2u, int>> exit_closest;
    
    //generate 4 exits
    for(int i = 0; i < 4; ++i)
    {
        sf::Vector2<std::int64_t> adjacent = {position.x + dirs[i].x, position.y + dirs[i].y};
        if(abs(adjacent.x) > dungeon_size.x && abs(adjacent.y) > dungeon_size.y)
        {
            //skip if no exit
            continue;
        }

        //get exit random device
        std::size_t exit_seed = std::hash<std::string>{}(get_edge_hash(position, adjacent, dungeon_seed, dungeon_size));
        std::mt19937 rd_dev(exit_seed);
        std::uniform_int_distribution<std::uint32_t> dist(std::min(dungeon_min, 2 * (chunk_size / 5)),
                                                          chunk_size - std::min(dungeon_min, 2 * (chunk_size / 5)));
        int exit_pos = dist(rd_dev);
        std::shared_ptr<Room> n_exit = std::make_shared<Room>();

        if(dirs[i].x != 0)
        {
            n_exit->position.x = (dirs[i].x > 0) ? chunk_size - 1 : 0;
            n_exit->position.y = exit_pos;
            exit_closest.push_back({n_exit->position, {0, 0}, std::numeric_limits<int>().max()});
        }
        else if(dirs[i].y != 0)
        {
            n_exit->position.x = exit_pos;
            n_exit->position.y = (dirs[i].y > 0) ? chunk_size - 1 : 0;
            exit_closest.push_back({n_exit->position, {0, 0}, std::numeric_limits<int>().max()});
        }

        n_chunk->rooms.push_back(n_exit);
    }

    //prapare queue for room generation
    std::queue<std::tuple<std::shared_ptr<DungeonRect>, bool>> q;
    std::shared_ptr<DungeonRect> root = std::make_shared<DungeonRect>();
    root->rect = sf::IntRect({0, 0}, {static_cast<int>(chunk_size), static_cast<int>(chunk_size)});
    q.push({root, false});

    //get chunk hash and radnom device
    std::stringstream chunk_hash;
    chunk_hash << "Chunk_x" << position.x << "_y_" << position.y << "_Seed_" << dungeon_seed << "_"
               << "DungeonSize_" << dungeon_size.x << "_" << dungeon_size.y << "__";
    std::size_t chunk_seed = std::hash<std::string>{}(chunk_hash.str());
    std::mt19937 rd_dev(chunk_seed);
    std::bernoulli_distribution bern_dist(0.5);

    //helper for vector "normalization"
    auto sign = [](int v){if(v > 0) return 1; else if(v < 0) return -1; return 0;};

    while(!q.empty())
    {
        std::shared_ptr<Room> n_room;
        auto& [d, visited] = q.front();

        if(!visited)
        {
            if(!d->left && !d->right)
            {
                //dicide if not visited
                d->divide(rd_dev, (bern_dist(rd_dev) > 0.5 ? sf::Vector2i(1, 0) : sf::Vector2i(0, 1)), chunk_size);
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
                //if leaf then add a room
                const sf::Vector2u n_room_pos = {static_cast<unsigned int>(d->rect.getCenter().x),
                                                static_cast<unsigned int>(d->rect.getCenter().y)};
                n_room = std::make_shared<Room>(n_room_pos);

                n_chunk->rooms.push_back(n_room);
            }
        }
        else
        {
            if(d->left && d->right)
            {
                //connect rooms / rectangles
                sf::Vector2i l = d->left->rect.getCenter();
                sf::Vector2i r = d->right->rect.getCenter();
                sf::Vector2i dir = {sign(l.x - r.x), sign(l.y - r.y)};
                sf::Vector2i pos = l;

                int dx = sign(r.x - pos.x);
                while (pos.x != r.x)
                {
                    pos.x += dx;
                    n_room = std::make_shared<Corridor>();
                    auto n_corr = std::static_pointer_cast<Corridor>(n_room);
                    n_corr->position = {static_cast<unsigned int>(pos.x), static_cast<unsigned int>(pos.y)};
                    n_corr->vertical = false;
                    n_chunk->rooms.push_back(n_corr);
                }

                int dy = sign(r.y - pos.y);
                while (pos.y != r.y)
                {
                    pos.y += dy;
                    n_room = std::make_shared<Corridor>();
                    auto n_corr = std::static_pointer_cast<Corridor>(n_room);
                    n_corr->position = {static_cast<unsigned int>(pos.x), static_cast<unsigned int>(pos.y)};
                    n_corr->vertical = true;
                    n_chunk->rooms.push_back(n_corr);
                }
            }

            const sf::Vector2u rect_cnt = {static_cast<unsigned int>(d->rect.getCenter().x),
                                            static_cast<unsigned int>(d->rect.getCenter().y)};
            for(int i = 0; i < exit_closest.size(); i++)
            {
                auto& [e, r, c_min] = exit_closest[i];

                int dx = std::abs(static_cast<int>(rect_cnt.x) - static_cast<int>(e.x));
                int dy = std::abs(static_cast<int>(rect_cnt.y) - static_cast<int>(e.y));

                int dist = (e.x == 0 || e.x == chunk_size - 1) ? dy : dx;

                if(dist < c_min)
                {
                    r = rect_cnt;
                    c_min = dist;
                }
            }
            q.pop();
            continue;
        }

        visited = true;
    }

    for(int i = 0; i < exit_closest.size(); ++i)
    {
        auto& [e_pos, r_pos, d] = exit_closest[i];
        int dx = r_pos.x - e_pos.x;
        int dy = r_pos.y - e_pos.y;
        sf::Vector2i diff1, diff2;
        int j_m, k_m;
        bool do_ver;
        if(e_pos.x == 0 || e_pos.x == chunk_size - 1)
        {
            do_ver = false;
            j_m = std::abs(dx);
            k_m = std::abs(dy) - 1;
            diff1.x = std::abs(dx)/dx;
            diff1.y = 0;
            diff2.x = 0;
            diff2.y = std::abs(dy)/dy;
        }
        else
        {
            do_ver = true;
            j_m = std::abs(dy);
            k_m = std::abs(dx) - 1;
            diff1.x = 0;
            diff1.y = std::abs(dy)/dy;
            diff2.x = std::abs(dx)/dx;
            diff2.y = 0;
        }

        std::shared_ptr<Room> room;
        sf::Vector2i pos = {static_cast<int>(e_pos.x), static_cast<int>(e_pos.y)};
        bool do_end = false;
        auto is_corrior = [&](){return std::find_if(n_chunk->rooms.begin(), n_chunk->rooms.end(),
                                [&](const std::shared_ptr<Room>& r)
                                {
                                    return r->position.x == pos.x && r->position.y == pos.y;
                                }) != n_chunk->rooms.end();};
        for(int j = 0; j < j_m; ++j)
        {
            room = std::make_shared<Corridor>();
            auto n_corr = std::dynamic_pointer_cast<Corridor>(room);
            pos += diff1;
            if(is_corrior()) do_end = true;
            n_corr->position = {static_cast<unsigned int>(pos.x), static_cast<unsigned int>(pos.y)};
            n_corr->vertical = do_ver;
            n_chunk->rooms.push_back(n_corr);
            if(do_end)
            {
                auto ro = std::find_if(n_chunk->rooms.begin(), n_chunk->rooms.end(),
                                [&](const std::shared_ptr<Room>& r)
                                {
                                    return r->position.x == pos.x && r->position.y == pos.y;
                                });
                if(ro != n_chunk->rooms.end())
                {
                    if(std::dynamic_pointer_cast<Corridor>(*ro))
                        break;
                    else
                    {
                        n_chunk->rooms.erase(n_chunk->rooms.end() - 1);
                        break;
                    }
                }

            };
        }

        do_ver = !do_ver;
        if(std::find_if(n_chunk->rooms.begin(), n_chunk->rooms.end(),
                                [&](const std::shared_ptr<Room>& r)
                                {
                                    return r->position.x == pos.x && r->position.y == pos.y;
                                }) == n_chunk->rooms.end() - 1)
        {
            sf::Vector2u u_pos = {static_cast<unsigned int>(pos.x), static_cast<unsigned int>(pos.y)};
            n_chunk->rooms.back() = std::make_shared<Room>(u_pos);
        }

        for(int k = 0; k < k_m; ++k)
        {
            room = std::make_shared<Corridor>();
            auto n_corr = std::dynamic_pointer_cast<Corridor>(room);
            pos += diff2;
            if(is_corrior()) do_end = true;
            if(do_end) break;
            n_corr->position = {static_cast<unsigned int>(pos.x), static_cast<unsigned int>(pos.y)};
            n_corr->vertical = do_ver;
            n_chunk->rooms.push_back(n_corr);
        }
    }

    return n_chunk;
}
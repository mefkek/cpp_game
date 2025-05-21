#include "DungeonManager/ChunkGenerator.hpp"
#include "DungeonManager/Room.hpp"
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <queue>
#include <random>
#include <sstream>
#include <cmath>

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

std::shared_ptr<Chunk> ChunkGenerator::operator()(sf::Vector2<std::int64_t> position)
{
    if(abs(position.x) > dungeon_size.x || abs(position.y) > dungeon_size.y)
    {
        const std::string msg = "Position: " + std::to_string(position.x) + ", " + std::to_string(position.y) + " is out of range";
        throw std::runtime_error(msg);
    }

    std::array<sf::Vector2i, 4> dirs = {sf::Vector2i(1, 0), {0, 1}, {-1, 0}, {0, -1}};
    std::shared_ptr<Chunk> n_chunk = std::make_shared<Chunk>();
    
    for(int i = 0; i < 4; ++i)
    {
        sf::Vector2<std::int64_t> adjacent = {position.x + dirs[i].x, position.y + dirs[i].y};
        if(abs(adjacent.x) > dungeon_size.x && abs(adjacent.y) > dungeon_size.y)
        {
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
        }
        else if(dirs[i].y != 0)
        {
            n_exit->position.x = exit_pos;
            n_exit->position.y = (dirs[i].y > 0) ? chunk_size - 1 : 0;
        }

        n_chunk->rooms.push_back(n_exit);
    }

    std::queue<std::tuple<std::shared_ptr<DungeonRect>, bool>> q;
    std::shared_ptr<DungeonRect> root = std::make_shared<DungeonRect>();
    root->rect = sf::IntRect({0, 0}, {static_cast<int>(chunk_size), static_cast<int>(chunk_size)});
    q.push({root, false});

    std::stringstream chunk_hash;
    chunk_hash << "Chunk_x" << position.x << "_y_" << position.y << "_Seed_" << dungeon_seed << "_"
               << "DungeonSize_" << dungeon_size.x << "_" << dungeon_size.y << "__";
    std::size_t chunk_seed = std::hash<std::string>{}(chunk_hash.str());
    std::mt19937 rd_dev(chunk_seed);
    std::bernoulli_distribution bern_dist(0.5);

    while(!q.empty())
    {
        auto& [d, visited] = q.front();

        if(visited && d->left && d->right)
        {
            auto sign = [](int v){if(v > 0) return 1; else if(v < 0) return -1; return 0;};
            sf::Vector2i l = d->left->rect.getCenter();
            sf::Vector2i r = d->right->rect.getCenter();
            std::cout << "L " << l.x << ' ' << l.y << '\n';
            std::cout << "R " << r.x << ' ' << r.y << '\n';
            sf::Vector2i dir = {sign(l.x - r.x), sign(l.y - r.y)};
            sf::Vector2i pos = l;

            int dx = sign(r.x - pos.x);
            while (pos.x != r.x)
            {
                pos.x += dx;
                auto n_corr = std::make_shared<Corridor>();
                n_corr->position = {static_cast<unsigned int>(pos.x), static_cast<unsigned int>(pos.y)};
                n_corr->vertical = false;
                n_chunk->rooms.push_back(n_corr);
            }

            int dy = sign(r.y - pos.y);
            while (pos.y != r.y)
            {
                pos.y += dy;
                auto n_corr = std::make_shared<Corridor>();
                n_corr->position = {static_cast<unsigned int>(pos.x), static_cast<unsigned int>(pos.y)};
                n_corr->vertical = true;
                n_chunk->rooms.push_back(n_corr);
            }

            q.pop();
            continue;
        }

        if(!d->left && !d->right && !visited)
        {
            d->divide(rd_dev, (bern_dist(rd_dev) > 0.5 ? sf::Vector2i(1, 0) : sf::Vector2i(0, 1)), chunk_size);
        }
        else if(visited)
        {
            q.pop();
            continue;
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
            std::shared_ptr<Room> n_room = std::make_shared<Room>
                                           (sf::Vector2u(static_cast<unsigned int>(d->rect.getCenter().x),
                                           static_cast<unsigned int>(d->rect.getCenter().y)));

            n_chunk->rooms.push_back(n_room);
        }

        visited = true;
    }

    std::cout << "Gen ended\n";
    return n_chunk;
}
#include "DungeonManager/ChunkGenerator.hpp"
#include "DungeonManager/Room.hpp"
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <queue>
#include <random>
#include <sstream>
#include <cmath>

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
        long long min_div_size = std::min(dungeon_min, chunk_size / 5);
        if(min_div_size % 2 != 0)
        {
            ++min_div_size;
        }

        if (d_bound <= 2 * min_div_size)
        {
            return;
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

        long long min_div_size = std::min(dungeon_min, chunk_size / 5);
        if(min_div_size % 2 != 0)
        {
            ++min_div_size;
        }

        if (horizontal)
        {
            if (split_pos <= offset.y || split_pos >= offset.y + bounds.y)
            {
                return;   
            }

            sf::IntRect l_rect{offset, sf::Vector2i(bounds.x, split_pos)};
            sf::IntRect r_rect{sf::Vector2i(offset.x, offset.y + split_pos),
                               sf::Vector2i(bounds.x, bounds.y - split_pos)};

            left = std::make_shared<DungeonRect>();
            left->rect = l_rect;

            right = std::make_shared<DungeonRect>();
            right->rect = r_rect;
        }
        else
        {
            if (split_pos <= offset.x || split_pos >= offset.x + bounds.x)
            {
                return;
            }

            sf::IntRect l_rect{offset, sf::Vector2i(split_pos, bounds.y)};
            sf::IntRect r_rect{sf::Vector2i(offset.x + split_pos, offset.y),
                               sf::Vector2i(bounds.x - split_pos, bounds.y)};

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
    std::vector<std::shared_ptr<DungeonRect>> d_rects;
    std::shared_ptr<DungeonRect> root = std::make_shared<DungeonRect>();
    root->rect = sf::IntRect({0, 0}, {static_cast<int>(chunk_size), static_cast<int>(chunk_size)});
    d_rects.push_back(root);

    for(int i = 0; i < 4; ++i)
    {
        sf::Vector2<std::int64_t> adjacent = {position.x + dirs[i].x, position.y + dirs[i].y};
        std::vector<std::shared_ptr<DungeonRect>> buffer;

        //get exit random device
        std::size_t exit_seed = std::hash<std::string>{}(get_edge_hash(position, adjacent, dungeon_seed, dungeon_size));
        std::mt19937 rd_dev(exit_seed);
        std::uniform_int_distribution<std::uint32_t> dist(std::min(dungeon_min, chunk_size / 5),
                                                          chunk_size - std::min(dungeon_min, chunk_size / 5));
        int split_d = dist(rd_dev);
        for(auto& rect : d_rects)
        {
            if(!rect->left && !rect->right)
            {
                bool horizontal = (dirs[i].y != 0);
                rect->divide_along(split_d, horizontal, chunk_size);
                if(rect->left)
                {
                    buffer.push_back(rect->left);
                }
                if(rect->right)
                {
                    buffer.push_back(rect->right);
                }
            }
        }

        for(auto& b : buffer)
        {
            d_rects.push_back(b);
        }
    }

    std::queue<std::shared_ptr<DungeonRect>> q;
    for(auto& rect : d_rects)
    {
        q.push(rect);
    }
    
    std::shared_ptr<Chunk> n_chunk = std::make_shared<Chunk>();
    std::stringstream chunk_hash;
    chunk_hash << "Chunk_x" << position.x << "_y_" << position.y << "_Seed_" << dungeon_seed << "_"
               << "DungeonSize_" << dungeon_size.x << "_" << dungeon_size.y << "__";
    std::size_t chunk_seed = std::hash<std::string>{}(chunk_hash.str());
    std::mt19937 rd_dev(chunk_seed);
    std::bernoulli_distribution bern_dist(0.5);

    while(!q.empty())
    {
        std::shared_ptr<DungeonRect>& d = q.front();

        if(!d->left && !d->right)
        {
            d->divide(rd_dev, (bern_dist(rd_dev) > 0.5 ? sf::Vector2i(1, 0) : sf::Vector2i(0, 1)), chunk_size);
        }
        if(d->left)
        {
            q.push(d->left);
        }
        if(d->right)
        {
            q.push(d->right);
        }
        if(!d->left && !d->right)
        {
            std::shared_ptr<Room> n_room = std::make_shared<Room>
                                           (sf::Vector2u(static_cast<unsigned int>(d->rect.getCenter().x),
                                           static_cast<unsigned int>(d->rect.getCenter().y)));

            n_chunk->rooms.push_back(n_room);
        }

        q.pop();
    }

    return n_chunk;
}
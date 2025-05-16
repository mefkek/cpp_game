#include "DungeonManager/ChunkGenerator.hpp"
#include "DungeonManager/Room.hpp"
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <queue>
#include <random>
#include <sstream>
#include <cmath>

ChunkGenerator::ChunkGenerator(unsigned int chunk_size, sf::Vector2u dungeon_size, std::size_t dungeon_seed)
                               : chunk_size(chunk_size), dungeon_size(dungeon_size), dungeon_seed(dungeon_seed) {}

//helper function
std::string get_edge_hash(sf::Vector2<std::int64_t> a, sf::Vector2<std::int64_t> b, std::int64_t dungeon_seed)
{
    if (a.x > b.x || (a.x == b.x && a.y > b.y)) {
        std::swap(a, b);
    }

    std::stringstream ss;
    ss << "Chunk1_" << a.x << "_" << a.y << "Chunk2_" << b.x << "_" << b.y;
    ss << "_Seed_" << dungeon_seed;
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
        const unsigned int min_div_size = std::min(5u, chunk_size / 5);
        if (d_bound <= 2 * min_div_size)
        {
            return;
        }

        std::uniform_int_distribution dist(min_div_size, d_bound - min_div_size);
        std::bernoulli_distribution bern_dist(0.5);
        unsigned int d_pos = dist(rd_dev);

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

            left->divide(rd_dev, (bern_dist(rd_dev) ? sf::Vector2i(1, 0) : sf::Vector2i(0, 1)), chunk_size);
            right->divide(rd_dev, (bern_dist(rd_dev) ? sf::Vector2i(1, 0) : sf::Vector2i(0, 1)), chunk_size);
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

    //for(int i = 0; i < 4; ++i)
    {
        /*
        const sf::Vector2i& dir = dirs[i];
        sf::Vector2<std::int64_t> adjacent = {position.x + dir.x, position.y + dir.y};

        generate seed for edge of two chunks
        std::size_t exit_seed = std::hash<std::string>{}(get_edge_hash(position, adjacent, dungeon_seed));
        std::mt19937 rd_dev(exit_seed);
        std::uniform_int_distribution<std::uint32_t> dist(1, chunk_size - 2);
        unsigned int exit_pos = dist(rd_dev);

        /*
        //create exit
        //boundary check
        bool within_boundaries = true;
        if(std::abs(adjacent.x) > (dungeon_size.x / 2) || std::abs(adjacent.y) > (dungeon_size.y / 2))
        {
            within_boundaries = false;
        }

        sf::Vector2<std::uint32_t> room_pos;
        if (dir.x != 0) 
        {
            if(within_boundaries)
            {
                room_pos.x = (dir.x > 0) ? chunk_size - 1 : 0;
                room_pos.y = exit_pos;
            }

            
        }
        else 
        {
            if(within_boundaries)
            {
                room_pos.x = exit_pos;
                room_pos.y = (dir.y > 0) ? chunk_size - 1 : 0;
            }
        }
        */
        auto c_rect = std::make_shared<DungeonRect>();
        c_rect->rect = sf::IntRect({0, 0}, {static_cast<int>(chunk_size), static_cast<int>(chunk_size)});
        std::stringstream chunk_hash;
        chunk_hash << "Chunk_x" << position.x << "_y_" << position.y << "_Seed_" << dungeon_seed << "__";
        std::size_t chunk_seed = std::hash<std::string>{}(chunk_hash.str());
        std::mt19937 rd_dev(chunk_seed);
        c_rect->divide(rd_dev, {0,1}, chunk_size);
        std::queue<std::shared_ptr<DungeonRect>> q;
        q.push(c_rect);

        std::shared_ptr<Chunk> n_chunk = std::make_shared<Chunk>();
        while(!q.empty())
        {
            if(q.front()->left == nullptr && q.front()->right == nullptr)
            {
                std::shared_ptr n_room = std::make_shared<Room>(sf::Vector2u(static_cast<unsigned int>(q.front()->rect.getCenter().x), static_cast<unsigned int>(q.front()->rect.getCenter().y)));
                n_chunk->rooms.push_back(n_room);
            }

            if(q.front()->left != nullptr)
            {
                q.push(q.front()->left);
            }
            if(q.front()->right != nullptr)
            {
                q.push(q.front()->right);
            }

            q.pop();
        }

        return n_chunk;
    }
}
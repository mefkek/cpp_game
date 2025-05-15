#include "Nodes/DungeonManager.hpp"
#include "Nodes/Application.hpp"    //maybe for debug only
#include "Nodes/RenderManager.hpp"
#include <algorithm>
#include <random>
#include <tuple>
#include <sstream>
#include <string>
#include <cmath>

void DungeonManager::initialize()
{
    this->dungeon_seed = 10;    //for debugging only
    this->dungeon_size = {4, 4};
    this->chunk_size = 10;
}

DungeonManager::Room::Room(sf::Vector2<std::uint32_t> pos, std::vector<sf::Vector2i> exits)
                          : position(pos), exits(exits) {}

std::shared_ptr<DungeonManager::Chunk> DungeonManager::get_chunk(sf::Vector2<std::int64_t> position)
{
    if(abs(position.x) > dungeon_size.x / 2 || abs(position.y) > dungeon_size.y / 2)
    {
        const std::string msg = "Position: " + std::to_string(position.x) +
                                ", " + std::to_string(position.y) + " is out of range";
        throw std::runtime_error(msg);
    }

    std::vector<sf::Vector2i> dirs = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};

    std::shared_ptr<Chunk> new_chunk = std::make_shared<Chunk>();

    std::vector<std::tuple<std::shared_ptr<Room>, int>> room_map;   //room | parent
    std::array<std::shared_ptr<Room>, 4> top_rooms;

    for(int i = 0; i < 4; ++i)
    {
        //boundary check
        const sf::Vector2i& dir = dirs[i];
        sf::Vector2<std::int64_t> adjacent = {position.x + dir.x, position.y + dir.y};
        if(std::abs(adjacent.x) >= (chunk_size / 2) || std::abs(adjacent.y) >= (chunk_size / 2))
        {
            new_chunk->exits[i] = nullptr;
            top_rooms[i] = nullptr;
            continue;
        }

        //generate seed for edge of two chunks
        std::stringstream exit_hash;
        exit_hash << "Chunk1_";
        if(position.x < adjacent.x)
        {
            exit_hash << position.x << '_' << position.y << adjacent.x << '_' << adjacent.y;
        }
        else
        {
            exit_hash << adjacent.x << '_' << adjacent.y <<  position.x << '_' << position.y;
        }
        exit_hash << "_Seed_" << dungeon_seed;

        std::size_t exit_seed = std::hash<std::string>{}(exit_hash.str());
        std::mt19937 rd_dev(exit_seed);
        std::uniform_int_distribution<std::uint32_t> dist(1, chunk_size - 2);

        //create exit
        sf::Vector2<std::uint32_t> room_pos;
        if (dir.x != 0) 
        {
            room_pos.x = (dir.x > 0) ? chunk_size - 1 : 0;
            room_pos.y = dist(rd_dev);
        }
        else 
        {
            room_pos.x = dist(rd_dev);
            room_pos.y = (dir.y > 0) ? chunk_size - 1 : 0;
        }

        std::shared_ptr<Room> n_room = std::make_shared<Room>(room_pos, std::vector({dir, -dir}));
        new_chunk->exits[i] = n_room;
        room_map.push_back({n_room, i});
        std::shared_ptr<Room> other = std::make_shared<Room>
        (
            sf::Vector2<std::uint32_t>(room_pos.x - dir.x, room_pos.y - dir.y),
            std::vector({dir})
        );
        room_map.push_back({other, i});
        top_rooms[i] = other;
    }

    std::stringstream chunk_hash;
    chunk_hash << "Chunk1_" << position.x << "_" << position.y << "_Seed_" << dungeon_seed << "_";
    std::size_t chunk_seed = std::hash<std::string>{}(chunk_hash.str());
    std::mt19937 rd_dev(chunk_seed);
    std::uniform_int_distribution<std::uint32_t> dist(0, 3);

    int max_attemps = 10000;
    while((top_rooms[0] || top_rooms[1] || top_rooms[2] || top_rooms[3]) && (--max_attemps > 0))
    {
        for (int i = 0; i < 4; ++i)
        {
            std::shared_ptr<Room> current = top_rooms[i];
            if(!current)
            {
                continue;
            }

            sf::Vector2i dir = dirs[dist(rd_dev)];
            sf::Vector2<std::int64_t> n_pos =
            {
                current->position.x + dir.x,
                current->position.y + dir.y
            };

            // Bounds check, with keeping the borders clean
            if (n_pos.x < 1 || n_pos.x >= chunk_size - 1 || n_pos.y < 1 || n_pos.y >= chunk_size - 1)
            {
                continue;
            }

            bool found = false;
            int other = -1;

            for (const auto& [m_room, parent] : room_map)
            {
                if (m_room->position.x == n_pos.x && m_room->position.y == n_pos.y)
                {
                    found = true;
                    other = parent;
                    break;
                }
            }

            if (found)
            {
                if (other == i)
                {
                    continue;
                }
                else if (other >= 0)
                {
                    top_rooms[i] = nullptr;
                    top_rooms[other] = nullptr;
                }
                continue;
            }

            // If not found, create new room
            sf::Vector2<std::uint32_t> m_pos = 
            {
                static_cast<std::uint32_t>(n_pos.x),
                static_cast<std::uint32_t>(n_pos.y)
            };

            auto new_room = std::make_shared<Room>(m_pos, std::vector{dir});
            room_map.push_back({new_room, i});
            top_rooms[i] = new_room;
        }
    }

    for(const auto& [room, parent] : room_map)
    {
        new_chunk->rooms.push_back(room);
    }

    return new_chunk;
}

void DungeonManager::display_chunk(std::shared_ptr<Chunk> debug)
{
    if(debug)
    {
        const sf::Vector2u su = Application::instance().get_manager<RenderManager>()->get_render_texture_size("Debug_ui");
        const sf::Vector2f ss = {su.x, su.y};
        const sf::Vector2f pos = {ss.x / 2.f - ((chunk_size / 2) * 20.f), ss.y / 2.f - ((chunk_size / 2) * 20.f)};    //offset from corner
        static std::vector<std::shared_ptr<sf::Drawable>> debug_drawables;
        debug_drawables.clear();
        for(auto& room : debug->rooms)
        {
            sf::Vector2f n_pos;
            n_pos.x = pos.x + room->position.x * 20.f;
            n_pos.y = pos.y + room->position.y * 20.f;
            std::shared_ptr<sf::RectangleShape> room_rect = std::make_shared<sf::RectangleShape>(sf::Vector2(15.f, 15.f));
            room_rect->setPosition(n_pos);
            room_rect->setFillColor(sf::Color::Green);
            debug_drawables.emplace_back(room_rect);
            Application::instance().get_manager<RenderManager>()->add_drawable("Debug_ui", room_rect);
        }
    }
}
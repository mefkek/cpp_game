#include "Nodes/DungeonManager.hpp"
#include <algorithm>
#include <random>
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

DungeonManager::Chunk DungeonManager::get_chunk(sf::Vector2<std::int64_t> position)
{
    if(abs(position.x) > dungeon_size.x / 2 || abs(position.y) > dungeon_size.y / 2)
    {
        const std::string msg = "Position: " + std::to_string(position.x) +
                                ", " + std::to_string(position.y) + " is out of range";
        throw std::runtime_error(msg);
    }

    std::vector<sf::Vector2i> dirs = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};

    std::array<std::vector<std::shared_ptr<Room>>, 4> room_lines;

    Chunk new_chunk;

    //create exits
    std::stringstream exit_hash;
    for(int i = 0; i < 4; ++i)
    {
        std::int64_t n_x = position.x + dirs[i].x;
        std::int64_t n_y = position.y + dirs[i].y;

        if(n_x >= 0 && n_x < chunk_size && n_y >= 0 && n_y < chunk_size)
        {
            if(position.x <= n_x)
            {
                exit_hash << "Chunk1_" << position.x << "_" << position.y
                          << "Chunk2_" << n_x << "_" << n_y
                          << "_Seed_" << dungeon_seed;
            }
            else
            {
                exit_hash << "Chunk1_" << n_x << "_" << n_y
                          << "Chunk2_" << position.x << "_" << position.y
                          << "_Seed_" << dungeon_seed;
            }

            std::size_t exit_seed = std::hash<std::string>{}(exit_hash.str());
            std::mt19937 gen(exit_seed);
            std::uniform_int_distribution<std::uint32_t> dist(1, chunk_size - 2);
            sf::Vector2<std::uint32_t> room_pos;
            if(std::abs(dirs[i].x) > 0)
            {
                room_pos.x = chunk_size - (chunk_size * std::max(0, -dirs[i].x));
                room_pos.y = dist(gen);
            }
            else
            {
                room_pos.y = chunk_size - (chunk_size * std::max(0, -dirs[i].y));
                room_pos.x = dist(gen);
            }

            new_chunk.exits[i] = std::make_shared<DungeonManager::Room>(room_pos,
                                                                        std::vector<sf::Vector2i>({dirs[i], -dirs[i]}));
            new_chunk.rooms.push_back(new_chunk.exits[i]);
            room_lines[i].push_back(new_chunk.exits[i]);
        }
        else
        {
            new_chunk.exits[i] = nullptr; //invalid chunk
        }
    }    

    std::stringstream to_hash;
    to_hash << "Chunk_" << position.x << "_" << position.y << "_Seed_" << dungeon_seed;
    std::size_t chunk_seed = std::hash<std::string>{}(to_hash.str());
    std::mt19937 gen(chunk_seed);
    std::uniform_int_distribution<std::uint8_t> direction_selector(0, 3);
    std::uniform_real_distribution<float> chance_gen(0.f, 1.f);

    std::array<bool, 4> connected = {false, false, false, false};
    for(int i = 0; i < 4; ++i)
    {
        if(!new_chunk.exits[i])
        {
            connected[i] = true;
        }
    }

    uint32_t max_attempts = 10000;
    while(!(connected[0] && connected[1] && connected[2] && connected[3]) && (--max_attempts > 0))
    {
        for(int i = 0; i < 4; ++i)
        {
            if(connected[i])
            {
                continue;
            }

            sf::Vector2i d;
            if (room_lines[i].empty())
            {
                throw std::runtime_error("Chunk genaration failed, invalid exits generated");
            }
            if(room_lines[i].size() == 1)
            {
                if(new_chunk.rooms.back()->position.x == 0)
                {
                    d = {0, 1};
                }
                else if(new_chunk.rooms.back()->position.x == chunk_size)
                {
                    d = {0, -1};
                }
                else if(new_chunk.rooms.back()->position.y == 0)
                {
                    d = {-1, 0};
                }
                else
                {
                    d = {1, 0};
                }
            }
            else
            {
                d = dirs[direction_selector(gen)];
            }

            auto it = std::find_if(room_lines[i].begin(), room_lines[i].end(),
                                   [&d](const std::weak_ptr<Room> o)
                                    {
                                        if (auto ptr = o.lock())
                                        {
                                            return ptr->position.x == d.x && ptr->position.y == d.y;
                                        }
                                        return false;
                                    });

            if(it != room_lines[i].end())
            {
                for(int j = 0; j < 4; ++j)
                {
                    if(i == j)
                    {
                        continue;
                    }

                    auto rl_it = std::find_if(room_lines[j].begin(), room_lines[j].end(),
                                   [&d](const std::weak_ptr<Room> o)
                                    {
                                        if (auto ptr = o.lock())
                                        {
                                            return ptr->position.x == d.x && ptr->position.y == d.y;
                                        }
                                        return false;
                                    });
                    
                    if(rl_it != room_lines[j].end())
                    {
                        connected[i] = true;
                        connected[j] = true;
                        break;
                    }
                }

                if(connected[i])
                {
                    continue;
                }

                sf::Vector2<std::uint32_t> n_pos;
                n_pos.x = room_lines[i].back()->position.x + d.x;
                n_pos.y = room_lines[i].back()->position.y + d.y;
                room_lines[i].push_back(std::make_shared<Room>(n_pos, std::vector<sf::Vector2i>({sf::Vector2i(-d.x, -d.y)})));
            }
        }
    }

    if(max_attempts <= 0)
    {
        Logger::log(Logger::MessageType::Warning, "Chunk generator infinite loop protection triggerd");
    }

    return new_chunk;
}
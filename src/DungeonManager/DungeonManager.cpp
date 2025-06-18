#include "DungeonManager/DungeonManager.hpp"
#include "Nodes/Application.hpp"    //maybe for debug only
#include "Nodes/RenderManager.hpp"
#include "Events/TimedEvent.hpp"
#include <stack>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <cmath>
#include <algorithm>
#include <thread>
#include <sstream>

constexpr sf::Vector2f debug_disp_scale = {5.f, 5.f};

DungeonManager::DungeonManager(const TextureAtlas& atlas, std::size_t seed, sf::Vector2u dungeon_size,
                               unsigned int chunk_size, sf::Vector2i player_pos_d, sf::Vector2i player_pos_c)
                               : dungeon_seed(seed), dungeon_size(dungeon_size), chunk_size(chunk_size),
                                 pos_dungeon(player_pos_d)
{
    this->loaded_chunks.resize(9);

    chunk_getter = std::make_unique<ChunkGenerator>(chunk_size, dungeon_size, dungeon_seed);
    visualizer = std::make_unique<RoomVisualizer>(atlas);
    if(player_pos_c != sf::Vector2i{0, 0})
    {
        loaded = true;
        pos_chunk = player_pos_c;
    }
}

void DungeonManager::reload_chunks()
{
    std::array<sf::Vector2i, 9> dirs = 
    {
        sf::Vector2i{-1, 1}, {0, 1}, {1,1},
        {-1, 0}, {0, 0}, {1, 0},
        {-1, -1}, {0, -1}, {1, -1}
    };

    std::vector<chunk_p> new_loaded_chunks(9);
    std::vector<std::thread> threads;
    for(int i = 0; i < dirs.size(); ++i)
    {
        sf::Vector2i target_pos = pos_dungeon + dirs[i];
        auto it = std::find_if(loaded_chunks.begin(), loaded_chunks.end(), [&](const chunk_p& o){return o.first == target_pos && o.second;});
        if(it != loaded_chunks.end())
        {
            new_loaded_chunks[i] = *it;
            continue;
        }

        threads.emplace_back([i, this, &dirs, &new_loaded_chunks, target_pos]()
        {
            new_loaded_chunks[i] = {target_pos, chunk_getter->operator()(target_pos)};
        });
    }

    for(auto& t : threads)
    {
        t.join();
    }

    loaded_chunks = std::move(new_loaded_chunks);

    current_chunk = loaded_chunks[4].second;
}

void DungeonManager::initialize()
{
    reload_chunks();

    if(loaded)
    {
        visualizer->visualize(loaded_chunks, chunk_size, pos_chunk, debug_disp_scale);
        return;
    }
    sf::Vector2i closest_room = {0, 0};
    float closest_distance = std::numeric_limits<float>::max();
    sf::Vector2i chunk_mid = {static_cast<int>(chunk_size) / 2, static_cast<int>(chunk_size) / 2};
    for(auto& [x, row] : current_chunk->rooms)
    {
        for(auto& [y, room] : row)
        {
            if(std::dynamic_pointer_cast<Corridor>(room))
            {
                continue;
            }
            float dx = static_cast<float>(chunk_mid.x) - static_cast<float>(x);
            float dy = static_cast<float>(chunk_mid.y) - static_cast<float>(y);

            float dist = std::sqrt(static_cast<float>(dx * dx + dy * dy));
            if(dist < closest_distance)
            {
                closest_distance = dist;
                closest_room = {x, y};
            }
        }
    }
    
    pos_chunk = closest_room;    
    visualizer->visualize(loaded_chunks, chunk_size, pos_chunk, debug_disp_scale);
}

bool DungeonManager::move(sf::Vector2i diff)
{
    if(block_movement)
    {
        return false;
    }
    if(!current_chunk->rooms.count(pos_chunk.x) || !current_chunk->rooms[pos_chunk.x].count(pos_chunk.y))
    {
        return false;
    }
    auto room = current_chunk->rooms[pos_chunk.x][pos_chunk.y];
    if(std::find(room->exits.begin(), room->exits.end(), diff) != room->exits.end())
    {
        pos_chunk += diff;
    }
    else
    {
        return false;
    }

    bool changed = false;
    if(pos_chunk.x < 0)
    {
        --pos_dungeon.x;
        pos_chunk.x = chunk_size - 1;
        changed = true;
    }
    else if(pos_chunk.x > chunk_size - 1)
    {
        ++pos_dungeon.x;
        pos_chunk.x = 0;
        changed = true;
    }
    else if(pos_chunk.y < 0)
    {
        --pos_dungeon.y;
        pos_chunk.y = chunk_size - 1;
        changed = true;
    }
    else if(pos_chunk.y > chunk_size - 1)
    {
        ++pos_dungeon.y;
        pos_chunk.y = 0;
        changed = true;
    }

    if(changed)
    {
        reload_chunks();
        visualizer->visualize(loaded_chunks, chunk_size, pos_chunk, debug_disp_scale);
    }
    else
    {
        visualizer->move(diff, current_chunk, pos_chunk);
    }

    return true;
}

std::string DungeonManager::as_string()
{
    std::stringstream ss;

    ss << dungeon_seed << " " << dungeon_size.x << " " << dungeon_size.y << " "
       << chunk_size << " " << pos_dungeon.x << " " << pos_dungeon.y << " "
       << pos_chunk.x << " " << pos_chunk.y;

    return ss.str();
}
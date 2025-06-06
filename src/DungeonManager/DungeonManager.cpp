#include "DungeonManager/DungeonManager.hpp"
#include "Nodes/Application.hpp"    //maybe for debug only
#include "Nodes/RenderManager.hpp"
#include <stack>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <cmath>
#include <algorithm>

constexpr sf::Vector2f debug_disp_scale = {5.f, 5.f};

void DungeonManager::initialize()
{
    this->dungeon_seed = 10;    //for debugging only
    this->dungeon_size = {255, 255};
    this->chunk_size = 32;
    pos_dungeon = {0, 0};

    static TextureAtlas atlas("Textures/Tileset.png");   //debug only

    chunk_getter = std::make_unique<ChunkGenerator>(chunk_size, dungeon_size, dungeon_seed);
    visualizer = std::make_unique<RoomVisualizer>(atlas);
    this->chunk = chunk_getter->operator()(pos_dungeon);

    sf::Vector2i closest_room = {0, 0};
    float closest_distance = std::numeric_limits<float>::max();
    for(auto& [x, row] : chunk->rooms)
    {
        for(auto& [y, room] : row)
        {
            if(std::dynamic_pointer_cast<Corridor>(room))
            {
                continue;
            }
            float dist = std::sqrt(static_cast<float>(x * x + y * y));
            if(dist < closest_distance)
            {
                closest_distance = dist;
                closest_room = {x, y};
            }
        }
    }

    pos_chunk = closest_room;
    visualizer->visualize(chunk, chunk_size, pos_chunk, debug_disp_scale);
}

bool DungeonManager::move(sf::Vector2i diff)
{
    auto room = chunk->rooms[pos_chunk.x][pos_chunk.y];
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
        chunk = chunk_getter->operator()(pos_dungeon);
        visualizer->visualize(chunk, chunk_size, pos_chunk, debug_disp_scale);
    }
    else
    {
        visualizer->move(diff);
    }

    Logger::log(Logger::MessageType::Info, "Pos: ", pos_chunk.x, ", ", pos_chunk.y);
    return true;
}
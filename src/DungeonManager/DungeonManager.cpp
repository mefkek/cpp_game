#include "DungeonManager/DungeonManager.hpp"
#include "Nodes/Application.hpp"    //maybe for debug only
#include "Nodes/RenderManager.hpp"
#include <stack>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <cmath>

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
            float dist = std::sqrt(static_cast<float>(x * x + y * y));
            if(dist < closest_distance)
            {
                closest_distance = dist;
                closest_room = {x, y};
            }
        }
    }

    pos_chunk = closest_room;
    visualizer->visualize(chunk, chunk_size, pos_chunk, {5.f, 5.f});
}

bool DungeonManager::move(sf::Vector2i diff)
{
    sf::Vector2i n_pos = pos_chunk + diff;

    if(chunk->rooms.count(n_pos.x) && chunk->rooms[n_pos.x].count(n_pos.y))
    {
        visualizer->move(diff);
        pos_chunk = n_pos;
    }
    else
    {
        std::shared_ptr<Chunk> n_chunk;
        sf::Vector2i t_pos = pos_dungeon;
        sf::Vector2i new_pos_chunk = n_pos;

        if(n_pos.x < 0)
        {
            t_pos.x -= 1;
            new_pos_chunk.x = chunk_size - 1;
        }
        else if(n_pos.x >= chunk_size)
        {
            t_pos.x += 1;
            new_pos_chunk.x = 0;
        }

        if(n_pos.y < 0)
        {
            t_pos.y -= 1;
            new_pos_chunk.y = chunk_size - 1;
        }
        else if(n_pos.y >= chunk_size)
        {
            t_pos.y += 1;
            new_pos_chunk.y = 0;
        }

        n_chunk = chunk_getter->operator()(t_pos);

        if(!n_chunk || !n_chunk->rooms.count(new_pos_chunk.x) || !n_chunk->rooms[new_pos_chunk.x].count(new_pos_chunk.y))
        {
            return false;
        }

        chunk = n_chunk;
        pos_dungeon = t_pos;
        pos_chunk = new_pos_chunk;
        visualizer->visualize(n_chunk, chunk_size, pos_chunk, {5.f, 5.f});
    }

    return true;
}
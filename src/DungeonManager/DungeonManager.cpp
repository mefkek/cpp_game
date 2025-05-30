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

    static TextureAtlas atlas("build/runtime_files/Dungeon_Tileset.png");   //debug only

    chunk_getter = std::make_unique<ChunkGenerator>(chunk_size, dungeon_size, dungeon_seed);
    visualizer = std::make_unique<RoomVisualizer>(atlas);
    this->chunk = chunk_getter->operator()({0, 0});
}

bool DungeonManager::move(sf::Vector2i diff)
{
    sf::Vector2i n_pos = pos_chunk + diff;

    if(!chunk->rooms.count(n_pos.x) || !chunk->rooms[n_pos.x].count(n_pos.y))
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
    }

    else
    {
        pos_chunk = n_pos;
    }

    visualizer->visualize(chunk->rooms[pos_chunk.x][pos_chunk.y]->type, {0, 0}, {5.f, 5.f});
    return true;
}
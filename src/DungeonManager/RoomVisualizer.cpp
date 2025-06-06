#include "DungeonManager/RoomVisualizer.hpp"
#include "Nodes/RenderManager.hpp"
#include <cmath>
#include <random>

RoomVisualizer::RoomVisualizer(TextureAtlas& tileset)
    : tileset(tileset), room_tilemap(std::make_shared<Tilemap>())
{
    room_tilemap->rotate(sf::degrees(180.f));
}

void RoomVisualizer::visualize(std::shared_ptr<Chunk> chunk, unsigned int chunk_size, sf::Vector2i chunk_pos, sf::Vector2f scale)
{
    room_tiles.clear();
    room_tiles.resize(chunk_size * chunk_size);
    for(int y = 0; y < chunk_size; ++y)
    {
        for(int x = 0; x < chunk_size; ++x)
        {
            if(chunk->rooms.count(x) && chunk->rooms[x].count(y))
            {
                auto room = chunk->rooms[x][y];
                if(x == chunk_pos.x && y == chunk_pos.y)
                {
                    room_tiles[y*chunk_size+x] = {10, 3};
                }
                if(auto c_ptr = std::dynamic_pointer_cast<Corridor>(room))
                {
                    if(c_ptr->vertical)
                    {
                        room_tiles[y * chunk_size + x] = {10, 1};
                    }
                    else
                    {
                        room_tiles[y * chunk_size + x] = {10, 2};
                    }
                }
                else
                {
                    room_tiles[y * chunk_size + x] = {10, 0};
                }
            }
            else
            {
                room_tiles[y * chunk_size + x] = {10, 4};
            }
        }
    }

    room_tilemap->load(tileset, {16, 16}, room_tiles.data(), chunk_size, chunk_size);
    room_tilemap->setScale({scale.x, scale.y});
    sf::Vector2f size = {chunk_size * 16.f * scale.x, chunk_size * 16.f * scale.x};
    room_tilemap->setPosition({Application::instance().get_manager<RenderManager>()->get_render_texture("ddun").getSize().x / 2.f + chunk_pos.x * 16.f * scale.x,
                               Application::instance().get_manager<RenderManager>()->get_render_texture("ddun").getSize().y / 2.f + chunk_pos.y * 16.f * scale.y});

    Application::instance().get_manager<RenderManager>()->add_drawable("ddun", room_tilemap);

    tileset.set_rect(player_sprite, {10, 3});
    player_sprite->setScale(scale);
    player_sprite->setPosition({Application::instance().get_manager<RenderManager>()->get_render_texture("ddun").getSize().x / 2.f - 16.f * scale.x,
                               Application::instance().get_manager<RenderManager>()->get_render_texture("ddun").getSize().y / 2.f - 16.f * scale.y});
    Application::instance().get_manager<RenderManager>()->add_drawable("ddun", player_sprite);
}

void RoomVisualizer::move(sf::Vector2i diff)
{
    sf::Vector2f scale = room_tilemap->getScale();
    room_tilemap->move({diff.x * 16.f * scale.x, diff.y * 16.f * scale.y});
}
#include "DungeonManager/RoomVisualizer.hpp"
#include "Nodes/RenderManager.hpp"
#include <cmath>
#include <random>

RoomVisualizer::RoomVisualizer(TextureAtlas& tileset)
    : tileset(tileset), room_tilemap(std::make_shared<Tilemap>())
{
    room_tilemap->rotate(sf::degrees(180.f));
}

void RoomVisualizer::visualize(const std::array<chunk_p, 9>& chunks, unsigned int chunk_size, sf::Vector2i chunk_pos, sf::Vector2f scale)
{
    room_tiles.clear();
    room_tiles.resize((3 * chunk_size) * (3 * chunk_size));
    for(int i = 0; i < 3; ++i)
    {
        int swapped_i = 2 - i; // Swap top and bottom rows
        for(int j = 0; j < 3; ++j)
        {
            auto& [pos, chunk] = chunks[swapped_i * 3 + j];

            for(int y = 0; y < chunk_size; ++y)
            {
                for(int x = 0; x < chunk_size; ++x)
                {
                    int global_x = j * chunk_size + x;
                    int global_y = i * chunk_size + y;
                    int index = global_y * (chunk_size * 3) + global_x;

                    if(chunk->rooms.count(x) && chunk->rooms[x].count(y))
                    {
                        auto room = chunk->rooms[x][y];
                        if(auto c_ptr = std::dynamic_pointer_cast<Corridor>(room))
                        {
                            if(c_ptr->vertical)
                            {
                                room_tiles[index] = {10, 1};
                            }
                            else
                            {
                                room_tiles[index] = {10, 2};
                            }
                        }
                        else
                        {
                            room_tiles[index] = {10, 0};
                        }
                    }
                    else
                    {
                        room_tiles[index] = {10, 4};
                    }
                }
            }
        }
    }

    auto render = Application::instance().get_manager<RenderManager>();
    room_tilemap->load(tileset, {16, 16}, room_tiles.data(), chunk_size * 3, chunk_size * 3);
    room_tilemap->setScale({scale.x, scale.y});
    sf::Vector2f chunk_offset = {(chunk_size * 2.f) * 16.f * scale.x, (chunk_size * 2.f) * 16.f * scale.y};
    sf::Vector2f player_offset = {(chunk_size - chunk_pos.x) * 16.f * scale.x, (chunk_size - chunk_pos.y) * 16.f * scale.y};
    room_tilemap->setPosition({render->get_render_texture("ddun").getSize().x / 2.f + chunk_offset.x - player_offset.x,
                               render->get_render_texture("ddun").getSize().y / 2.f + chunk_offset.y - player_offset.y});

    render->add_drawable("ddun", room_tilemap);

    tileset.set_rect(player_sprite, {10, 3});
    player_sprite->setScale(scale);
    player_sprite->setPosition({render->get_render_texture("ddun").getSize().x / 2.f - 16.f * scale.x,
                                render->get_render_texture("ddun").getSize().y / 2.f - 16.f * scale.y});
    render->add_drawable("ddun", player_sprite);
}

void RoomVisualizer::move(sf::Vector2i diff)
{
    sf::Vector2f scale = room_tilemap->getScale();
    room_tilemap->move({diff.x * 16.f * scale.x, diff.y * 16.f * scale.y});
}
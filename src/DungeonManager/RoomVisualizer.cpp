#include "DungeonManager/RoomVisualizer.hpp"
#include "Nodes/RenderManager.hpp"
#include <cmath>
#include <random>
#include <algorithm>

RoomVisualizer::RoomVisualizer(const TextureAtlas& tileset)
    : tileset(tileset), map_tilemap(std::make_shared<Tilemap>()), room_tilemap(std::make_shared<Tilemap>())
{
    std::mt19937 rd(2137);
    std::uniform_int_distribution rand_wall(1, 4);
    std::uniform_int_distribution rand_wall_h(1, 3);
    std::uniform_int_distribution rand_floor_x(6, 9);
    std::uniform_int_distribution rand_floor_y(0, 2);

    map_tilemap->rotate(sf::degrees(180.f));

    auto render = Application::instance().get_manager<RenderManager>();

    render->add_layer("dungeon_map", 188, {1920u, 1020u});
    auto& map_texture = render->get_render_texture("dungeon_map");
    auto& map_sprite = render->get_render_sprite("dungeon_map");

    render->add_layer("dungeon_room", 187, {640u / 2u, 360u / 2u});
    auto& room_texture = render->get_render_texture("dungeon_room");
    auto& room_sprite = render->get_render_sprite("dungeon_room");

    auto window_size = static_cast<sf::Vector2f>(Application::instance().get_window().getSize());
    map_sprite.setScale({window_size.x * 0.3f / map_texture.getSize().x,
                         window_size.y * 0.3f / map_texture.getSize().y});
    map_sprite.setPosition({window_size.x * 0.7f, 0.f});

    room_sprite.setScale({window_size.x * 0.7f / room_texture.getSize().x,
                          window_size.y * 0.7f / room_texture.getSize().y});
    room_sprite.setPosition({0.f, 0.f});

    for(int i = 0; i < 12; ++i)
    {
        corridor_tiles[i] = {rand_wall(rd), 0};
    }

    for(int i = 1; i < 6; ++i)
    {
        for(int j = 0; j < 12; ++j)
        {
            corridor_tiles[i * 12 + j] = {rand_floor_x(rd), rand_floor_y(rd)};
        }
    }

    for(int i = 0; i < 12; ++i)
    {
        corridor_tiles[6 * 12 + i] = {rand_wall(rd), 0};
    }

    for(int i = 0; i < 5 * 2; ++i)
    {
        for(int j = 0; j < 6 * 2; ++j)
        {
            if(i == 0 || i == 9)
            {
                room_tiles[i * 12 + j] = {rand_wall(rd), ((i == 0) ? 0 : 4)};
            }
            else if(j == 0 || j == 11)
            {
                room_tiles[i * 12 + j] = {((j == 0) ? 0 : 5), rand_wall_h(rd)};
            }
            else
            {
                room_tiles[i * 12 + j] = {rand_floor_x(rd), rand_floor_y(rd)};
            }
        }
    }

    room_tiles[0] = {0, 0};
    room_tiles[11] = {5, 0};
    room_tiles[10 * 10 + 8] = {0, 4};
    room_tiles[11 * 10 + 9] = {5, 4};

    render->add_drawable("dungeon_room", room_tilemap);
}

void RoomVisualizer::display_minimap(const std::vector<chunk_p>& chunks, unsigned int chunk_size, sf::Vector2i chunk_pos, sf::Vector2f scale)
{
    map_tiles = std::vector<sf::Vector2i>((3 * chunk_size) * (3 * chunk_size), {10, 8});
    
    for(int i = 0; i < 3; ++i)
    {
        int swapped_i = 2 - i;
        for(int j = 0; j < 3; ++j)
        {
            auto& [pos, chunk] = chunks[swapped_i * 3 + j];
            if(!chunk)
            {
                continue;
            }

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
                                if(c_ptr->exits.size() == 4)
                                {
                                    map_tiles[index] = {10, 4};
                                }
                                else if(std::find(c_ptr->exits.begin(), c_ptr->exits.end(), sf::Vector2i{-1, 0}) != c_ptr->exits.end())
                                {
                                    map_tiles[index] = {10, 3};
                                }
                                else if(std::find(c_ptr->exits.begin(), c_ptr->exits.end(), sf::Vector2i{1, 0}) != c_ptr->exits.end())
                                {
                                    map_tiles[index] = {10, 2};
                                }
                                else
                                {
                                    map_tiles[index] = {10, 1};
                                }
                            }
                            else
                            {
                                if(c_ptr->exits.size() == 4)
                                {
                                    map_tiles[index] = {10, 4};
                                }
                                else if(std::find(c_ptr->exits.begin(), c_ptr->exits.end(), sf::Vector2i{0, -1}) != c_ptr->exits.end())
                                {
                                    map_tiles[index] = {10, 6};
                                }
                                else if(std::find(c_ptr->exits.begin(), c_ptr->exits.end(), sf::Vector2i{0, 1}) != c_ptr->exits.end())
                                {
                                    map_tiles[index] = {10, 7};
                                }
                                else
                                {
                                    map_tiles[index] = {10, 5};
                                }
                            }
                        }
                        else
                        {
                            map_tiles[index] = {10, 0};
                        }
                    }
                }
            }
        }
    }

    auto render = Application::instance().get_manager<RenderManager>();
    auto& layer = render->get_render_texture("dungeon_map");
    map_tilemap->load(tileset, {16, 16}, map_tiles.data(), chunk_size * 3, chunk_size * 3);
    map_tilemap->setScale({scale.x, scale.y});
    sf::Vector2f chunk_offset = {(chunk_size * 2.f) * 16.f * scale.x, (chunk_size * 2.f) * 16.f * scale.y};
    sf::Vector2f player_offset = {(chunk_size - chunk_pos.x) * 16.f * scale.x, (chunk_size - chunk_pos.y) * 16.f * scale.y};
    map_tilemap->setPosition({layer.getSize().x / 2.f + chunk_offset.x - player_offset.x,
                               layer.getSize().y / 2.f + chunk_offset.y - player_offset.y});

    render->add_drawable("dungeon_map", map_tilemap);

    tileset.set_rect(player_sprite, {10, 9});
    player_sprite->setScale(scale);
    player_sprite->setPosition({layer.getSize().x / 2.f - 16.f * scale.x, layer.getSize().y / 2.f - 16.f * scale.y});
    render->add_drawable("dungeon_map", player_sprite);
}

void RoomVisualizer::display_room(const std::shared_ptr<Chunk> current_chunk, sf::Vector2i chunk_pos)
{
    sf::Vector2f tilemap_size;
    if(current_chunk->rooms.count(chunk_pos.x) && current_chunk->rooms[chunk_pos.x].count(chunk_pos.y))
    {
        if(auto r_ptr = current_chunk->rooms[chunk_pos.x][chunk_pos.y])
        {
            if(auto c_ptr = std::dynamic_pointer_cast<Corridor>(r_ptr))
            {
                room_tilemap->load(tileset, {16, 16}, corridor_tiles.data(), 12, 7);
                tilemap_size = {12 * 16.f, 7 * 16.f};
            }
            else
            {
                room_tilemap->load(tileset, {16, 16}, room_tiles.data(), 6 * 2, 5 * 2);
                tilemap_size = {12 * 16.f, 10 * 16.f};
            }
        }
    }

    auto& layer = Application::instance().get_manager<RenderManager>()->get_render_texture("dungeon_room");
    sf::Vector2f size = layer.getView().getSize();
    room_tilemap->setOrigin(tilemap_size / 2.f);
    room_tilemap->setPosition(size / 2.f);
}

void RoomVisualizer::visualize(const std::vector<chunk_p>& chunks, unsigned int chunk_size, sf::Vector2i chunk_pos, sf::Vector2f scale)
{
    display_minimap(chunks, chunk_size, chunk_pos, scale);
    display_room(chunks[4].second, chunk_pos);
}

void RoomVisualizer::move(sf::Vector2i diff, std::shared_ptr<Chunk> current_chunk, sf::Vector2i chunk_pos)
{
    sf::Vector2f scale = map_tilemap->getScale();
    map_tilemap->move({diff.x * 16.f * scale.x, diff.y * 16.f * scale.y});
    display_room(current_chunk, chunk_pos);
}

RoomVisualizer::~RoomVisualizer()
{
    if(Application::instance().get_window().isOpen())
    {
        auto render = Application::instance().get_manager<RenderManager>();

        render->remove_layer("dungeon_map");
        render->remove_layer("dungeon_room");
    }
}
#include "DungeonManager/RoomVisualizer.hpp"
#include "Nodes/RenderManager.hpp"
#include <random>

RoomVisualizer::RoomVisualizer(TextureAtlas& tileset)
    : tileset(tileset)
{
    std::mt19937 rd(2137);
    std::uniform_int_distribution rand_wall(1, 4);
    std::uniform_int_distribution rand_floor_x(6, 9);
    std::uniform_int_distribution rand_floor_y(0, 2);

    for(int i = 0; i < 12; ++i)
    {
        room_tiles.emplace_back(rand_wall(rd), 0);
    }

    for(int i = 0; i < 5; ++i)
    {
        for(int j = 0; j < 12; ++j)
        {
            room_tiles.emplace_back(rand_floor_x(rd), rand_floor_y(rd));
        }
    }

    for(int i = 0; i < 12; ++i)
    {
        room_tiles.emplace_back(rand_wall(rd), 4);
    }

    room_tilemap = std::make_shared<Tilemap>();
    room_tilemap->load(tileset, {16, 16}, room_tiles.data(), 12, 7);
    room_tilemap->setScale({10.f, 10.f});
    room_tilemap->setPosition({0.f, 30.f});
    Application::instance().get_manager<RenderManager>()->add_drawable("ddun", room_tilemap);
}

void RoomVisualizer::visualize(const Room::RoomType type, sf::Vector2f position, sf::Vector2f scale)
{
    tileset.set_rect(special_element, {4, 10});
    Application::instance().get_manager<RenderManager>()->add_drawable("ddun", special_element);
}
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
    this->dungeon_size = {256, 256};
    this->chunk_size = 10;

    chunk_getter = std::make_unique<ChunkGenerator>(chunk_size, dungeon_size, dungeon_seed);
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
            n_pos.y = pos.y + (chunk_size - 1 - room->position.y) * 20.f;   //flip axis
            std::shared_ptr<sf::RectangleShape> room_rect = std::make_shared<sf::RectangleShape>(sf::Vector2(15.f, 15.f));
            room_rect->setPosition(n_pos);
            room_rect->setFillColor(sf::Color::Green);
            debug_drawables.emplace_back(room_rect);
            Application::instance().get_manager<RenderManager>()->add_drawable("Debug_ui", room_rect);
        }
    }
}
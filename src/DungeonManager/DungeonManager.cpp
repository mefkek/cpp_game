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
    this->chunk_size = 32;

    chunk_getter = std::make_unique<ChunkGenerator>(chunk_size, dungeon_size, dungeon_seed);
}

void DungeonManager::display_chunk(std::shared_ptr<Chunk> debug)
{
    if(debug)
    {
        const sf::Vector2u su = Application::instance().get_manager<RenderManager>()->get_render_texture_size("Debug_ui");
        const sf::Vector2f ss = {static_cast<float>(su.x), static_cast<float>(su.y)};
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

            if (auto r = std::dynamic_pointer_cast<Corridor>(room))
            {
                room_rect->setFillColor(sf::Color::Yellow);
                if (r->vertical)
                {
                    room_rect->setScale({0.4f, 1.f});
                }
                else
                {
                    room_rect->setScale({1.f, 0.4f});
                }

                room_rect->setOrigin(room_rect->getGeometricCenter());
            }
            else
            {
                room_rect->setFillColor(sf::Color::Green);
                room_rect->setOrigin(room_rect->getGeometricCenter());
            }

            debug_drawables.emplace_back(room_rect);
            Application::instance().get_manager<RenderManager>()->add_drawable("Debug_ui", room_rect);
        }
    }
}
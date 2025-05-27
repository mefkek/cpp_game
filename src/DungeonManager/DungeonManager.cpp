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

    chunk_getter = std::make_unique<ChunkGenerator>(chunk_size, dungeon_size, dungeon_seed);
}

void DungeonManager::display_chunk(std::shared_ptr<Chunk> debug)
{
    if (debug)
    {
        const sf::Vector2u su = Application::instance().get_manager<RenderManager>()->get_render_texture_size("ddun");
        const sf::Vector2f ss = { static_cast<float>(su.x), static_cast<float>(su.y) };

        const float padding_factor = 0.9f;

        float max_dim = static_cast<float>(chunk_size); 

        float max_room_width = (ss.x * padding_factor) / max_dim;
        float max_room_height = (ss.y * padding_factor) / max_dim;
        float scale = std::min(max_room_width, max_room_height);

        sf::Vector2f room_size(scale, scale);

        sf::Vector2f total_chunk_size = { room_size.x * max_dim, room_size.y * max_dim };
        sf::Vector2f pos = { (ss.x - total_chunk_size.x) / 2.f, (ss.y - total_chunk_size.y) / 2.f };

        static std::vector<std::shared_ptr<sf::Drawable>> debug_drawables;
        debug_drawables.clear();
        for (const auto& [pos_x, row] : debug->rooms)
        {
            for(const auto& [pos_y, room] : row)
            {
                float r_pos_x = pos.x + pos_x * room_size.x;
                float r_pos_y = pos.y + (chunk_size - 1 - pos_y) * room_size.y;

                std::shared_ptr<sf::RectangleShape> room_rect = std::make_shared<sf::RectangleShape>(room_size);
                room_rect->setPosition({r_pos_x, r_pos_y});
                if (auto r = std::dynamic_pointer_cast<Corridor>(room))
                {
                    room_rect->setFillColor(sf::Color::Yellow);
                    if (r->vertical)
                    {
                        room_rect->setScale({ 0.4f, 1.f });
                    }
                    else
                    {
                        room_rect->setScale({ 1.f, 0.4f });
                    }
                    room_rect->setOrigin(room_rect->getGeometricCenter());
                }
                else if(std::dynamic_pointer_cast<Room>(room))
                {
                    room_rect->setFillColor(sf::Color::Green);
                    room_rect->setOrigin(room_rect->getGeometricCenter());
                }

                if(room_rect)
                {
                    debug_drawables.emplace_back(room_rect);
                    Application::instance().get_manager<RenderManager>()->add_drawable("ddun", room_rect);
                    for(auto exit : room->exits)
                    {
                        auto ex_shape = std::make_shared<sf::CircleShape>(room_size.x * 0.15f);
                        if(std::dynamic_pointer_cast<Corridor>(room))
                        {
                            ex_shape->setFillColor(sf::Color::Blue);
                        }
                        else
                        {
                            ex_shape->setScale({2.f, 2.f});
                            ex_shape->setFillColor(sf::Color::Red);
                        }
                        ex_shape->setOrigin({ex_shape->getRadius(), ex_shape->getRadius()});

                        sf::Vector2f center = room_rect->getPosition();
                        sf::Vector2f offset(exit.x * room_rect->getSize().x / 2.f, -exit.y * room_rect->getSize().y / 2.f);
                        sf::Vector2f final_pos = center + offset;

                        ex_shape->setPosition(final_pos);

                        debug_drawables.push_back(ex_shape);
                        if(std::dynamic_pointer_cast<Corridor>(room))
                        {
                            Application::instance().get_manager<RenderManager>()->add_drawable("ddun_e_c", ex_shape);
                        }
                        else
                        {
                            Application::instance().get_manager<RenderManager>()->add_drawable("ddun_e_r", ex_shape);
                        }
                    }
                }
            }
        }
    }
}
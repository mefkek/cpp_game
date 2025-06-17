#pragma once
#include "GameStateBase.hpp"

class GameState : public GameStateBase
{
    private:
    unsigned long ev_id;
    std::weak_ptr<Label> pause_text;
    std::weak_ptr<Menu> pause_menu;

    public:
    GameState(std::size_t seed, sf::Vector2u dungeon_size, unsigned int chunk_size,
        sf::Vector2i player_pos_d = {0, 0}, sf::Vector2i player_pos_c = {0, 0});
    void initialize() override;
    void update(float delta) override;
    ~GameState();
};
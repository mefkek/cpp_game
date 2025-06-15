#pragma once
#include "Nodes/Node.hpp"
#include "Ui/Menu.hpp"
#include <array>

class GameStateBase : public Node
{
    public:
    GameStateBase() = default;
    virtual ~GameStateBase() = default;
};

class MainMenuState : public GameStateBase
{
    public:
    void initialize() override;
    void update(float delta) override;
};

class GameSetupState : public GameStateBase
{
    private:
    std::size_t seed;
    int selected_d_size = 0;
    std::array<unsigned int, 4> dungeon_sizes = {128, 256, 512, 1024};
    std::shared_ptr<Menu> menu_ptr;

    int selected_c_size = 0;
    std::array<unsigned int, 4> chunk_sizes = {16, 32, 64, 128};

    std::weak_ptr<SelectableLabel> seed_l, d_size_l, c_size_l;

    public:
    GameSetupState();
    void initialize() override;
    void update(float delta) override;
};

class GameState : public GameStateBase
{
    private:
    unsigned long ev_id;

    public:
    GameState(std::size_t seed, sf::Vector2u dungeon_size, unsigned int chunk_size,
        sf::Vector2i player_pos_d = {0, 0}, sf::Vector2i player_pos_c = {0, 0});
    void initialize() override;
    void update(float delta) override;
    ~GameState();
};
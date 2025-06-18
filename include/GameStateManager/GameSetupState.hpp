#pragma once
#include "GameStateBase.hpp"

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
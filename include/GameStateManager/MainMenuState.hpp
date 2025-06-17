#pragma once
#include "GameStateBase.hpp"

class MainMenuState : public GameStateBase
{
    public:
    void initialize() override;
    void update(float delta) override;
};
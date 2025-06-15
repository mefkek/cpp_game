#pragma once
#include "Nodes/Node.hpp"
#include "Ui/Menu.hpp"

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

class GameState : public GameStateBase
{
    public:
    void initialize() override;
};
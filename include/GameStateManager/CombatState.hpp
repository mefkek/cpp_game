#pragma once

#include "GameStateBase.hpp"
#include "Combat/CombatManager.hpp"
#include "Nodes/ActorManager.hpp"

class CombatState : public GameStateBase {
public:
    CombatState(std::shared_ptr<Actor> player, std::vector<std::shared_ptr<Actor>> enemies);
    void initialize() override;
    void update(float delta) override;

private:
    CombatManager manager_;
    std::weak_ptr<Label> infoLabel_;
};

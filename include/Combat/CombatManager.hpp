#pragma once

#include "Nodes/Actor.hpp"
#include <vector>
#include <algorithm>

struct Combatant {
    std::shared_ptr<Actor> actor;
    bool isEnemy;
};

class CombatManager {
public:
    CombatManager(std::shared_ptr<Actor> player, std::vector<std::shared_ptr<Actor>> enemies);
    // Returns false once combat is over
    bool update(float delta);

private:
    std::shared_ptr<Actor> player_;
    std::vector<std::shared_ptr<Actor>> enemies_;
    std::vector<Combatant> queue_;
    size_t currentIndex_ = 0;
    bool combatEnded_ = false;

    void initializeQueue();
    void advanceTurn();
    void handlePlayerTurn();
    void handleEnemyTurn(const std::shared_ptr<Actor>& enemy);
    void applyDamage(const std::shared_ptr<Actor>& attacker, const std::shared_ptr<Actor>& target);
};

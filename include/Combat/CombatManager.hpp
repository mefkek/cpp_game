#pragma once

#include "Nodes/Actor.hpp"
#include <memory>

// Manages static click combat; no speed or defense
class CombatManager {
public:
    CombatManager(std::shared_ptr<Actor> player,
                  std::shared_ptr<Actor> enemy)
        : player_(std::move(player)), enemy_(std::move(enemy)) {}

    // Player clicks to attack the enemy
    void attackEnemy() {
        int atk = player_->get_stat("attack");
        enemy_->changeStat("hp", -atk);
    }

    bool isEnemyDead() const {
        return enemy_->get_stat("hp") <= 0;
    }

private:
    std::shared_ptr<Actor> player_;
    std::shared_ptr<Actor> enemy_;
};


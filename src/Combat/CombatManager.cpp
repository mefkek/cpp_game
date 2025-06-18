#include "Combat/CombatManager.hpp"
#include "Nodes/Application.hpp"
#include "GameStateManager/GameStateManager.hpp"

CombatManager::CombatManager(std::shared_ptr<Actor> player, std::vector<std::shared_ptr<Actor>> enemies)
: player_(std::move(player)), enemies_(std::move(enemies)) {
    initializeQueue();
}

void CombatManager::initializeQueue() {
    queue_.clear();
    queue_.push_back({player_, false});
    for (const auto& e : enemies_) queue_.push_back({e, true});
    std::sort(queue_.begin(), queue_.end(), [](const Combatant& a, const Combatant& b) {
        return a.actor->get_stat("speed") > b.actor->get_stat("speed");
    });
    currentIndex_ = 0;
}

bool CombatManager::update(float delta) {
    if (combatEnded_) return false;
    if (queue_.empty()) {
        combatEnded_ = true;
        return false;
    }
    advanceTurn();
    return !combatEnded_;
}

void CombatManager::advanceTurn() {
    auto& turn = queue_[currentIndex_];
    if (!turn.isEnemy) {
        handlePlayerTurn();
    } else {
        handleEnemyTurn(turn.actor);
    }
    // Remove dead from queue
    queue_.erase(std::remove_if(queue_.begin(), queue_.end(), [](const Combatant& c) {
        return c.actor->get_stat("hp") <= 0;
    }), queue_.end());
    if (player_->get_stat("hp") <= 0 || queue_.size() <= 1) {
        combatEnded_ = true;
        return;
    }
    currentIndex_ = (currentIndex_ + 1) % queue_.size();
}

void CombatManager::handlePlayerTurn() {
    applyDamage(player_, enemies_.front());
}

void CombatManager::handleEnemyTurn(const std::shared_ptr<Actor>& enemy) {
    applyDamage(enemy, player_);
}

void CombatManager::applyDamage(const std::shared_ptr<Actor>& attacker, const std::shared_ptr<Actor>& target) {
    int atk = attacker->get_stat("attack");
    int def = target->get_stat("defense");
    int dmg = std::max(1, atk - def);
    target->changeStat("hp", -dmg);
}
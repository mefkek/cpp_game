#pragma once

#include "Nodes/Actor.hpp"
#include "Nodes/Application.hpp"
#include <unordered_map>

// Factory: spawns a static zombie with HP and attack
class EnemyFactory {
public:
    enum class Type { Zombie = 0 };

    static void init() {} // No-op: stats are hard-coded

    static std::shared_ptr<Actor> create(Type type, const std::string& layer) {
        struct Stats { int hp, attack; };
        static const std::unordered_map<Type, Stats> stats = {
            { Type::Zombie, {30, 5} }
        };
        const auto& cfg = stats.at(type);
        auto& atlas = Application::instance().atlas;
        int tileIndex = static_cast<int>(type);
        sf::Vector2i rect(tileIndex, 0);
        // Actor(name, race, behaviour, atlas, rect, layer)
        auto enemy = std::make_shared<Actor>(
            "Zombie",
            static_cast<ActorRaceEnum>(type),
            nullptr,
            atlas,
            rect,
            layer
        );
        enemy->changeStat("hp",     cfg.hp     - enemy->get_stat("hp"));
        enemy->changeStat("attack", cfg.attack - enemy->get_stat("attack"));
        return enemy;
    }
};


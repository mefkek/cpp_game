#pragma once

#include "Nodes/Actor.hpp"
#include <vector>
#include <unordered_map>

// Simple factory to create enemy Actors with default stats based on race
class EnemyFactory {
public:
    // Instantiate an enemy Actor for the given race, at given position
    static std::shared_ptr<Actor> createEnemy(ActorRaceEnum race, const sf::Vector2i& tilePosition, const std::string& layer) {
        // Define default stats per race
        static const std::unordered_map<ActorRaceEnum, std::unordered_map<std::string,int>> baseStats = {
            { ActorRaceEnum::Zombie,   {{"hp", 30}, {"attack", 5}, {"defense", 2}, {"speed", 3}} },
            { ActorRaceEnum::Skeleton, {{"hp", 20}, {"attack", 7}, {"defense", 3}, {"speed", 5}} },
            { ActorRaceEnum::Spider,   {{"hp", 15}, {"attack", 6}, {"defense", 1}, {"speed", 7}} },
            { ActorRaceEnum::Lich,     {{"hp", 50}, {"attack", 10}, {"defense", 5}, {"speed", 4}} }
        };
        int textureIndex = static_cast<int>(race);
        auto& atlas = Application::instance().atlas;
        auto enemy = std::make_shared<Actor>("Enemy", race, atlas, atlas.getTileRect(textureIndex), layer);
        // Initialize stats
        auto it = baseStats.find(race);
        if (it != baseStats.end()) {
            for (const auto& kv : it->second) {
                // Actor API: changeStat and get_stat
                enemy->changeStat(kv.first, kv.second - enemy->get_stat(kv.first));
            }
        }
        // Position on map: convert tilePosition to world coordinates
        enemy->setPosition(sf::Vector2f(tilePosition.x * atlas.tileSize, tilePosition.y * atlas.tileSize));
        return enemy;
    }
};

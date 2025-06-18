#pragma once

#include "GameStateManager/GameStateBase.hpp"
#include "Combat/EnemyFactory.hpp"
#include "Combat/CombatManager.hpp"
#include "Nodes/Application.hpp"
#include "GameStateManager/GameStateManager.hpp"
#include "GameStateManager/GameState.hpp"
#include <SFML/Window/Mouse.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <memory>

// Combat state: player clicks zombie to attack
class CombatState : public GameStateBase {
public:
    CombatState(std::shared_ptr<Actor> player,
                std::size_t seed,
                sf::Vector2u dungeonSize,
                unsigned int chunkSize,
                sf::Vector2i playerPosD,
                sf::Vector2i playerPosC)
        : player_(std::move(player)),
          seed_(seed), dungeonSize_(dungeonSize), chunkSize_(chunkSize),
          playerPosD_(playerPosD), playerPosC_(playerPosC) {}

    void initialize() override {
        EnemyFactory::init();
        enemy_ = EnemyFactory::create(EnemyFactory::Type::Zombie, "Entities");
        manager_ = std::make_unique<CombatManager>(player_, enemy_);
    }

    void update(float dt) override {
        auto& window = Application::instance().get_window();
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
            auto mp = sf::Mouse::getPosition(window);
            sf::Vector2f worldPos = window.mapPixelToCoords(mp);
            auto sprite = enemy_->getSprite();
            if (sprite && sprite->getGlobalBounds().contains(worldPos)) {
                manager_->attackEnemy();
                if (manager_->isEnemyDead()) {
                    Application::instance()
                        .get_manager<GameStateManager>()
                        ->change_state<GameState>(
                            seed_, dungeonSize_, chunkSize_, playerPosD_, playerPosC_
                        );
                }
            }
        }
    }

private:
    std::shared_ptr<Actor> player_;
    std::shared_ptr<Actor> enemy_;
    std::unique_ptr<CombatManager> manager_;
    std::size_t seed_;
    sf::Vector2u dungeonSize_;
    unsigned int chunkSize_;
    sf::Vector2i playerPosD_, playerPosC_;
};

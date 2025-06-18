#include "GameStateManager/CombatState.hpp"
#include "Nodes/Application.hpp"
#include "GameStateManager/GameStateManager.hpp"

CombatState::CombatState(std::shared_ptr<Actor> player, std::vector<std::shared_ptr<Actor>> enemies)
: manager_(std::move(player), std::move(enemies)) {}

void CombatState::initialize() {

    infoLabel_ = add_child<Label>("CombatUI", Application::instance().font, "Combat Started!", 24).lock();
}

void CombatState::update(float delta) {
    if (!manager_.update(delta)) {
        // Combat over: return to Dungeon GameState (using placeholder values)
        auto gsm = Application::instance().get_manager<GameStateManager>();
        gsm->change_state<GameState>(
            0,                    // seed placeholder
            sf::Vector2u{0,0},    // dungeon_size placeholder
            0,                    // chunk_size placeholder
            sf::Vector2i{0,0},    // player_pos_d placeholder
            sf::Vector2i{0,0}     // player_pos_c placeholder
        );
    }
}

#include <Nodes/ActorManager.hpp>
#include "Nodes/ActorRace.hpp"


/*void ActorManager::addActor(const std::shared_ptr<Actor>& actor) {
    actors.push_back(actor);

    // przyporządkowanie do drużyny wg rasy:
    switch (actor->getRace()) {
        case ActorRaceEnum::Zombie:
        case ActorRaceEnum::Skeleton:
        case ActorRaceEnum::Spider:
        case ActorRaceEnum::Lich:
            enemy_party.addActor(actor);
            break;
            // wszyscy inni to gracz
        default:
            player_party.addActor(actor);
            break;
    }
}

void ActorManager::removeActor(const std::shared_ptr<Actor>& actor) {
    actors.erase(
        std::remove(actors.begin(), actors.end(), actor),
        actors.end()
    );
    // usuń z obu drużyn
    player_party.removeActor(actor);
    enemy_party.removeActor(actor);

}*/
ActorManager::ActorManager() {
    // Tworzymy partie jako pod-węzły
    player_party = add_child<Party>();
    enemy_party  = add_child<Party>();
}

std::shared_ptr<Actor> ActorManager::addActor(ActorRaceEnum race, std::shared_ptr<ActorBehaviour> behav) {
    // Tworzymy aktora jako dziecko tego menedżera
    auto actor = add_child<Actor>(race, behav);

    // Dodajemy go do odpowiedniej drużyny
    switch (actor->getRace()) {
        case ActorRaceEnum::Zombie:
        case ActorRaceEnum::Skeleton:
        case ActorRaceEnum::Spider:
        case ActorRaceEnum::Lich:
            enemy_party->addActor(actor);
            break;
        default:
            player_party->addActor(actor);
            break;
    }

    return actor;
}

void ActorManager::update(float delta) {
    for (auto& child : children) {
        child->update(delta);
    }
}

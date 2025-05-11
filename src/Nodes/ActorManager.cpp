#include <Nodes/ActorManager.hpp>
#include "Nodes/ActorRace.hpp"

void ActorManager::addActor(const std::shared_ptr<Actor>& actor) {
    actors.push_back(actor);

    //przyporządkowanie do drużyny wg rasy:
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

}

void ActorManager::update(float delta) {
    // 1) Update wszystkich aktorów
    for (auto& actor : actors) {
        actor->update(delta);
    }

    // 2) Usuń martych
    std::vector<std::shared_ptr<Actor>> dead;
    for (auto& a : actors)
        if (a->isDead())
            dead.push_back(a);
    //  dla każdego martwego -> removeActor (czyści listy i Node)
    for (auto& a : dead)
        removeActor(a);
}

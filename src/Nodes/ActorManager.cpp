#include <Nodes/ActorManager.hpp>
//#include "Actor.hpp"

void ActorManager::addActor(const std::shared_ptr<Actor>& actor) {
    actors.push_back(actor);
}

void ActorManager::removeActor(const std::shared_ptr<Actor>& actor) {
    actors.erase(
        std::remove(actors.begin(), actors.end(), actor),
        actors.end()
    );
}

/*void ActorManager::update(float delta) {
    // 1) Update wszystkich aktorów
    for (auto& actor : actors) {
        actor->update(delta);
    }

    // 2) Usuń tych, którzy się "zabijają" (isDead()==true)
    actors.erase(
        std::remove_if(
            actors.begin(), actors.end(),
            [](const std::shared_ptr<Actor>& actor) {
                return actor->isDead();
            }
        ),
        actors.end()
    );
}*/
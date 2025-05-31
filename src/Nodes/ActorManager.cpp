#include "Nodes/ActorManager.hpp"
#include "Nodes/Party.hpp"
#include "Nodes/Actor.hpp"
#include <memory>
#include <algorithm>

void ActorManager::initialize() {
    // Safe to use shared_from_this() here 
    add_child<Party>();
    playerParty_ = std::dynamic_pointer_cast<Party>(get_children().back());

    add_child<Party>();
    enemyParty_ = std::dynamic_pointer_cast<Party>(get_children().back());
}

std::shared_ptr<Actor> ActorManager::addActor(
    ActorRaceEnum race,
    std::shared_ptr<ActorBehaviour> behaviour) {
    // 1) Instantiate the actor under this manager
    add_child<Actor>(race, behaviour);
    std::weak_ptr<Actor> actor = std::dynamic_pointer_cast<Actor>(get_children().back());

    // 2) Determine whether this actor is an enemy
    const bool isEnemy =
            race == ActorRaceEnum::Zombie ||
            race == ActorRaceEnum::Skeleton ||
            race == ActorRaceEnum::Spider ||
            race == ActorRaceEnum::Lich;

    // 3) Attach the same actor into the appropriate party
    auto targetParty = isEnemy ? enemyParty_ : playerParty_;
    if (auto party = targetParty) {
        // Push directly into party's children vector 
        party->get_children().push_back(actor.lock());
    } else {
        Logger::log(Logger::MessageType::Warning,
                    "ActorManager",
                    "target party not initialized for race ",
                    static_cast<int>(race));
    }

    return actor.lock();
}

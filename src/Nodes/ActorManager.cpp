#include "Nodes/ActorManager.hpp"

ActorManager::ActorManager()
{
    playerParty_ = add_child<Party>();
    enemyParty_  = add_child<Party>();
}

std::shared_ptr<Actor> ActorManager::addActor(
    ActorRaceEnum race,
    std::shared_ptr<ActorBehaviour> behaviour)
{
    // Create the actor as a direct child
    auto actor = add_child<Actor>(race, behaviour);

    // Also add to the appropriate party
    switch (race)
    {
        case ActorRaceEnum::Zombie:
        case ActorRaceEnum::Skeleton:
        case ActorRaceEnum::Spider:
        case ActorRaceEnum::Lich:
            enemyParty_->add_child<Actor>(race, behaviour);
            break;
        default:
            playerParty_->add_child<Actor>(race, behaviour);
            break;
    }

    return actor;
}

// No update override: Node::update(delta) will traverse parties & actors automatically.

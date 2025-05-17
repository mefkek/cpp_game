#include "Nodes/ActorManager.hpp"
#include "Nodes/Party.hpp"
#include "Nodes/Actor.hpp"
#include <memory>
#include <algorithm>

ActorManager::ActorManager()
{
    // Create the player party as a child of this manager
    add_child<Party>();
    {
        auto& kids = get_children();
        // The last child is the one we just added
        playerParty_ = std::dynamic_pointer_cast<Party>(kids.back());
    }

    // Create the enemy party as a child of this manager
    add_child<Party>();
    {
        auto& kids = get_children();
        enemyParty_ = std::dynamic_pointer_cast<Party>(kids.back());
    }
}

std::shared_ptr<Actor> ActorManager::addActor(
    ActorRaceEnum race,
    std::shared_ptr<ActorBehaviour> behaviour)
{
    // 1) Instantiate the actor under this manager
    add_child<Actor>(race, behaviour);
    std::shared_ptr<Actor> actor;
    {
        auto& kids = get_children();
        actor = std::dynamic_pointer_cast<Actor>(kids.back());
    }

    // 2) Determine whether this actor is an enemy based on race
    const bool isEnemy =
        race == ActorRaceEnum::Zombie ||
        race == ActorRaceEnum::Skeleton ||
        race == ActorRaceEnum::Spider ||
        race == ActorRaceEnum::Lich;

    // 3) Add the same actor into the appropriate party
    auto targetParty = isEnemy ? enemyParty_ : playerParty_;
    if (targetParty) {
        targetParty->add_child<Actor>(race, behaviour);
    } else {
        // Fallback warning if for some reason the party pointer is null
        Logger::log(Logger::MessageType::Warning,
                    "ActorManager: target party not initialized for race ",
                    static_cast<int>(race));
    }

    return actor;
}


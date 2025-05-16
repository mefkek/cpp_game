#pragma once

#include "Nodes/Node.hpp"
#include "Nodes/Party.hpp"
#include "Nodes/Actor.hpp"
#include "Nodes/ActorBehaviour.hpp"
#include <memory>

// Manages creation and grouping of Actor instances.
class ActorManager : public Node
{
public:
    ActorManager();
    ~ActorManager() override = default;

    // Spawn a new Actor and assign it to the appropriate Party.
    std::shared_ptr<Actor> addActor(ActorRaceEnum race,
                                    std::shared_ptr<ActorBehaviour> behaviour);

    // Rely on base Node::update(delta) for propagation; no override needed.

private:
    std::shared_ptr<Party> playerParty_;
    std::shared_ptr<Party> enemyParty_;
};

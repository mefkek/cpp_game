#pragma once

#include "Nodes/ContainerNode.hpp"
#include "Nodes/Node.hpp"
#include "Nodes/Party.hpp"
#include "Nodes/Actor.hpp"
#include "Nodes/ActorBehaviour.hpp"
#include <memory>

// Manages creation and grouping of Actor instances.
class ActorManager : public Node
{
public:
    ActorManager() = default;
    ~ActorManager() override = default;

    void update(float delta) override;
    void initialize() override;

    std::shared_ptr<Party> addParty(std::vector<std::shared_ptr<Actor>>& actors);



private:
    std::shared_ptr<Party> playerParty_;
    std::shared_ptr<Party> enemyParty_;
};


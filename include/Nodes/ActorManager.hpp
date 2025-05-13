#pragma once
#include "Nodes/Node.hpp"
#include "Nodes/Party.hpp"
#include "Nodes/Actor.hpp"
#include "Nodes/ActorBehaviour.hpp"
#include <memory>

class ActorManager : public Node, public std::enable_shared_from_this<ActorManager> {
public:
    ActorManager();
    ~ActorManager() override = default;
    // Tworzy i zwraca nowego aktora
    std::shared_ptr<Actor> addActor(ActorRaceEnum race, std::shared_ptr<ActorBehaviour> behav);
    //void addActor(const std::shared_ptr<Actor>& actor);

    //void removeActor(const std::shared_ptr<Actor>& actor);
    // Deleguje update po drzewie Node
    void update(float delta) override;
private:
    std::shared_ptr<Party> player_party;
    std::shared_ptr<Party> enemy_party;
    //Party player_party;
    //Party enemy_party;
    //std::vector<std::shared_ptr<Actor>> actors;
};




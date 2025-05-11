#pragma once
#include "Node.hpp"
#include "Party.hpp"
#include <vector>
#include <memory>
#include <algorithm> // std:remove_if
// Forward declaration Actor class
#include "Actor.hpp"

class ActorManager : public Node {
public:
    ActorManager() = default;
    ~ActorManager() override = default;
    // Adds a new actor
    void addActor(const std::shared_ptr<Actor>& actor);
    // Removes an actor
    void removeActor(const std::shared_ptr<Actor>& actor);
    // Called each frame: updates all actors and prunes dead ones
    void update(float delta) override;
private:
    Party player_party;
    Party enemy_party;
    std::vector<std::shared_ptr<Actor>> actors;
};




#pragma once

#include "Nodes/Node.hpp"
#include "Nodes/ActorRace.hpp"
#include "Nodes/ActorBehaviour.hpp"

#include <unordered_map>
#include <string>
#include <memory>

// A game entity with stats and behavior.
class Actor : public Node
{
public:
    // Construct with a race and initial behavior.
    Actor(ActorRaceEnum race, std::shared_ptr<ActorBehaviour> behaviour);
    ~Actor() override = default;

    // Called once per frame.
    void update(float delta) override;

    // Adjust a stat; logs a warning if the stat key doesn’t exist.
    void changeStat(const std::string& name, int delta);

    // Query this actor’s race.
    ActorRaceEnum getRace() const;

protected:
    // Hook after a successful stat change.
    virtual void onStatChange(const std::string& name, int newValue) {}

    // Hook when changing a non-existent stat.
    virtual void onStatChangeFailed(const std::string& name, int delta) {}

private:
    ActorRaceEnum                       race_;
    std::shared_ptr<ActorBehaviour>     behaviour_;
    std::unordered_map<std::string,int> stats_;
};

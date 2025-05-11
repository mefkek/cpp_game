#pragma once
#include "Node.hpp"
#include "ActorRace.hpp"
#include "ActorBehaviour.hpp"
#include <unordered_map>
#include <string>
#include <memory>

class Actor : public Node {
public:
    Actor(ActorRaceEnum raceIn, std::unique_ptr<ActorBehaviour> behav);
    virtual ~Actor() override = default;

    // co klatkę
    void update(float delta) override;

    // zmiana statystyki o delta
    void changeStat(const std::string& name, int delta);

    // dostęp do rasy
    ActorRaceEnum getRace() const { return race; }

    // czy martwy?
    bool isDead() const {
        auto it = stats.find("HP");
        return it != stats.end() && it->second <= 0;
    }

protected:
    // hook wywoływany za każdym razem, gdy stat zmieni wartość
    virtual void onStatChange(const std::string& name, int newValue) = 0;

    std::unordered_map<std::string,int>    stats;
    ActorRaceEnum                         race;
    std::unique_ptr<ActorBehaviour>       behaviour;
};

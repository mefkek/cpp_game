#include "Actor.hpp"

Actor::Actor(ActorRaceEnum raceIn, std::unique_ptr<ActorBehaviour> behav)
    : race(raceIn), behaviour(std::move(behav))
{
    // przykładowa inicjalizacja
    stats["HP"]     = 100;
    stats["Attack"] = 10;
}

void Actor::update(float delta) {
    if (behaviour)
        behaviour->update(*this, delta);
}

void Actor::changeStat(const std::string& name, int delta) {
    int& val = stats[name];
    val += delta;
    onStatChange(name, val);
}
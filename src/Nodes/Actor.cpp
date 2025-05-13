#include "Nodes/Actor.hpp"

Actor::Actor(ActorRaceEnum raceIn, std::shared_ptr<ActorBehaviour> behav)
    : race(raceIn), behaviour(behav)
{
    // przykładowa inicjalizacja
    stats["HP"]     = 100;
    stats["Attack"] = 10;
}

void Actor::update(float delta) {
    if (behaviour)
        behaviour->behave(*this);
}

void Actor::changeStat(const std::string& name, int delta) {
    // istnienie klucza:
    auto it = stats.find(name);
    if (it == stats.end()) {
        // rzucamy wyjątek, jeśli statystyka o podanej nazwie nie istnieje
        throw std::out_of_range("Stat '" + name + "' does not exist");
    }
    // aktualizacja wartości
    it->second += delta;
    // wywołanie hooka po zmianie
    onStatChange(name, it->second);
};

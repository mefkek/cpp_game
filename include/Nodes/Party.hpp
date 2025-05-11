#pragma once
#include <vector>
#include <memory>
#include <algorithm>
#include "Actor.hpp"

// Grupa aktorow
class Party {
public:
    void addActor(const std::shared_ptr<Actor>& a) {
        actors.push_back(a);
    }

    void removeActor(const std::shared_ptr<Actor>& a) {
        actors.erase(
            std::remove(actors.begin(), actors.end(), a),
            actors.end());
    }

    // Zwraca aktora
    std::shared_ptr<Actor> getActor(unsigned char idx) const {
        if (idx < actors.size())
            return actors[idx];
        return nullptr;
    }

    const std::vector<std::shared_ptr<Actor>>& getActors() const {
        return actors;
    }

private:
    std::vector<std::shared_ptr<Actor>> actors;
};

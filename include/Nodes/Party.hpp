#pragma once

#include "Nodes/Node.hpp"
#include "Nodes/Actor.hpp"

#include <vector>
#include <memory>
#include <algorithm>

// Grupa aktorów jako węzeł w drzewie sceny
class Party : public Node {
public:
    Party() = default;
    ~Party() override = default;

    // Dodaje aktora do grupy
    void addActor(const std::shared_ptr<Actor>& a) {
        actors.push_back(a);
    }

    // Usuwa aktora z grupy
    void removeActor(const std::shared_ptr<Actor>& a) {
        actors.erase(
            std::remove(actors.begin(), actors.end(), a),
            actors.end()
        );
    }

    // Deleguje update na wszystkich aktorów w grupie
    void update(float delta) override {
        for (auto& a : actors) {
            a->update(delta);
        }
    }

    // Dostęp do aktorów
    std::shared_ptr<Actor> getActor(size_t idx) const {
        return idx < actors.size() ? actors[idx] : nullptr;
    }
    const std::vector<std::shared_ptr<Actor>>& getActors() const {
        return actors;
    }

private:
    std::vector<std::shared_ptr<Actor>> actors;
};

#pragma once

#include <memory>
class Actor;

// Defines behavior for an Actor; takes a weak_ptr to avoid
// forcing ownership and prevent dangling references.
class ActorBehaviour {
public:
    virtual ~ActorBehaviour() = default;

    // Perform behavior on the given actor.
    // The actor pointer must be locked before use.
    virtual void behave(std::weak_ptr<Actor> actor_wp) = 0;
};
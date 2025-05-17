#pragma once

#include "Actor.hpp"
class  Actor;
// Defines per-frame behavior for an Actor.
class ActorBehaviour
{
public:
    virtual ~ActorBehaviour() = default;

    // Called each frame to drive the actor’s logic.
    virtual void behave(Actor& actor) = 0;
};
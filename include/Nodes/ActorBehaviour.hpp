#pragma once
class Actor;

// klasa abstrakcyjna
class ActorBehaviour {
public:
    virtual ~ActorBehaviour() = default;

    virtual void behave(Actor& actor) = 0;
};
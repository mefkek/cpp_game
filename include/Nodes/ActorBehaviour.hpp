#pragma once
class Actor;

// klasa abstrakcyjna
class ActorBehaviour {
public:
    virtual ~ActorBehaviour() = default;

    virtual void update(Actor& actor, float delta) = 0;
};
#include "Nodes/Actor.hpp"
#include "Utility/Logger.hpp"

Actor::Actor(ActorRaceEnum race, std::shared_ptr<ActorBehaviour> behaviour)
    : race_(race)
    , behaviour_(behaviour)
{
    // Default stat initialization
    stats_["HP"]     = 100;
    stats_["Attack"] = 10;
}

void Actor::update(float delta)
{
    if (behaviour_)
    {
        behaviour_->behave(std::dynamic_pointer_cast<Actor>(shared_from_this()));
    }
}

void Actor::changeStat(const std::string& name, int delta)
{
    auto it = stats_.find(name);
    if (it == stats_.end())
    {
        // Log a warning if the stat does not exist, then invoke failure hook
        Logger::log(
            Logger::MessageType::Warning,
            shared_from_this(),                   // <-- use shared_from_this() here
            " tried changing non-existent stat ",
            name
        );
        onStatChangeFailed(name, delta);
        return;
    }

    int newValue = it->second + delta;
    it->second = newValue;

    // Invoke success hook after stat modification
    onStatChange(name, newValue);
}

ActorRaceEnum Actor::getRace() const
{
    return race_;
}

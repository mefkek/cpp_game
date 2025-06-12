#include "Nodes/Application.hpp"
#include "Nodes/RenderManager.hpp"
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

Actor::Actor(ActorRaceEnum race, std::shared_ptr<ActorBehaviour> behaviour, const TextureAtlas& atlas,
    sf::Vector2i rect, const std::string& display_layer)
    : Actor(race, behaviour)
{
    atlas.set_rect(sprite_, rect);
    Application::instance().get_manager<RenderManager>()->add_drawable(display_layer, sprite_);
}

Actor::Actor(ActorRaceEnum race, std::shared_ptr<ActorBehaviour> behaviour, const TextureAtlas& atlas,
    sf::IntRect rect, const std::string& display_layer)
    : Actor(race, behaviour)
{
    atlas.set_rect(sprite_, rect);
    Application::instance().get_manager<RenderManager>()->add_drawable(display_layer, sprite_);
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

std::shared_ptr<sf::Sprite> Actor::getSprite() const
{
    return sprite_;
}

std::string Actor::toString() const
{
    // Format:
    //   <race as int> <numStats>\n
    //   <key1> <value1>\n
    //   <key2> <value2>\n
    //   …
    std::ostringstream oss;
    oss << static_cast<int>(race_) << ' ' << stats_.size() << '\n';
    for (const auto& kv : stats_)
    {
        oss << kv.first << ' ' << kv.second << '\n';
    }
    return oss.str();
}


#include "Nodes/Actor.hpp"
#include <sstream>
#include <stdexcept>

// … other includes and Actor methods …

void Actor::fromString(const std::string& data)
{
    std::istringstream iss(data);

    int raceInt = 0;
    size_t numStats = 0;

    // 1) Read the race (as integer) and the count of stats
    if (!(iss >> raceInt >> numStats))
    {
        throw std::runtime_error("Actor::fromString: failed to parse race/numStats");
    }

    // 2) Cast to ActorRaceEnum (assumes the integer is valid).
    //    If you need to guard against out-of-range values, you could add your own check here.
    race_ = static_cast<ActorRaceEnum>(raceInt);

    // 3) Read exactly 'numStats' key/value pairs
    std::unordered_map<std::string,int> newStats;
    newStats.reserve(numStats);

    for (size_t i = 0; i < numStats; ++i)
    {
        std::string key;
        int value;
        if (!(iss >> key >> value))
        {
            throw std::runtime_error("Actor::fromString: failed reading stat key/value");
        }
        newStats[key] = value;
    }

    // 4) If there is any leftover text, consider it an error
    std::string leftover;
    if (iss >> leftover)
    {
        throw std::runtime_error("Actor::fromString: extra data after reading stats");
    }

    // 5) Commit the newly parsed stats
    stats_ = std::move(newStats);
}

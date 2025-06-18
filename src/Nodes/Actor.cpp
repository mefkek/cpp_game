#include "Nodes/Application.hpp"
#include "Nodes/RenderManager.hpp"
#include "Nodes/Actor.hpp"
#include "Utility/Logger.hpp"

Actor::Actor(std::string actor_name, ActorRaceEnum race, std::shared_ptr<ActorBehaviour> behaviour)
    : race_(race)
    , behaviour_(behaviour)
    , name(actor_name)
{
    // Default stat initialization
    stats_["HP"]     = 100;
    stats_["Attack"] = 10;
}

Actor::Actor(std::string actor_name, ActorRaceEnum race, std::shared_ptr<ActorBehaviour> behaviour, const TextureAtlas& atlas,
    sf::Vector2i rect, const std::string& display_layer)
    : Actor(actor_name, race, behaviour)
{
    atlas.set_rect(sprite_, rect);
    Application::instance().get_manager<RenderManager>()->add_drawable(display_layer, sprite_);
}

Actor::Actor(std::string actor_name, ActorRaceEnum race, std::shared_ptr<ActorBehaviour> behaviour, const TextureAtlas& atlas,
    sf::IntRect rect, const std::string& display_layer)
    : Actor(actor_name, race, behaviour)
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

void Actor::change_stat(const std::string& name, int delta)
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
        return;
    }

    int newValue = it->second + delta;
    it->second = newValue;
}

ActorRaceEnum Actor::get_race() const
{
    return race_;
}

std::shared_ptr<sf::Sprite> Actor::get_sprite() const
{
    return sprite_;
}

const std::unordered_map<std::string,int>& Actor::get_stats()
{
    return stats_;
}

int Actor::get_stat(const std::string& name)
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
        return -1;
    }

    return it->second;
}

std::string Actor::to_string() const
{
    // Format:
    //   $$ACTOR_START$$\n
    //   <race as int> <numStats>\n
    //   <key1> <value1>\n
    //   <key2> <value2>\n
    //   …
    //   $$ACTOR_END&&\n
    std::ostringstream oss;
    oss << "$$ACTOR_START$$\n";
    oss << name << ' ' << static_cast<int>(race_) << ' ' << stats_.size() << '\n';
    for (const auto& kv : stats_)
    {
        oss << kv.first << ' ' << kv.second << '\n';
    }
    oss << "$$ACTOR_END&&\n";
    return oss.str();
}

std::string Actor::get_name()
{
    return name;
}

#include "Nodes/Actor.hpp"
#include <sstream>
#include <stdexcept>

// … other includes and Actor methods …

void Actor::from_string(const std::string& data)
{
    std::istringstream iss(data);

    std::string name_str;
    int raceInt = 0;
    size_t numStats = 0;

    // 1) Read the race (as integer) and the count of stats
    if (!(iss >> name_str >> raceInt >> numStats))
    {
        throw std::runtime_error("Actor::fromString: failed to parse race/numStats");
    }

    name = name_str;

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

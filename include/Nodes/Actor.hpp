#pragma once

#include <SFML/Graphics.hpp>

#include "Nodes/Node.hpp"
#include "Nodes/ActorRace.hpp"
#include "Nodes/ActorBehaviour.hpp"
#include "Nodes/TextureAtlas.hpp"

#include <unordered_map>
#include <string>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <ostream>
#include <istream>


// A game entity with stats and behavior.
class Actor : public Node
{
public:
    // Construct with a race and initial behavior.
    Actor(std::string actor_name, ActorRaceEnum race, std::shared_ptr<ActorBehaviour> behaviour, const TextureAtlas& atlas, sf::Vector2i rect, const std::string& display_layer);
    Actor(std::string actor_name, ActorRaceEnum race, std::shared_ptr<ActorBehaviour> behaviour, const TextureAtlas& atlas, sf::IntRect rect, const std::string& display_layer);
    ~Actor() override = default;

    // Called once per frame.
    void update(float delta) override;

    // Adjust a stat; logs a warning if the stat key doesn’t exist.
    void change_stat(const std::string& name, int delta);
    int get_stat(const std::string& name);

    // Query this actor’s race.
    ActorRaceEnum get_race() const;
    std::shared_ptr<sf::Sprite> get_sprite() const;
    const std::unordered_map<std::string,int>& get_stats();
    std::string get_name();

    std::string to_string() const;

    void from_string(const std::string& data);


protected:
    Actor(std::string actor_name, ActorRaceEnum race, std::shared_ptr<ActorBehaviour> behaviour);

private:
    ActorRaceEnum                       race_;
    std::shared_ptr<ActorBehaviour>     behaviour_;
    std::unordered_map<std::string,int> stats_;
    std::shared_ptr<sf::Sprite>         sprite_;
    std::string name;
};

struct DummyBehaviour : public ActorBehaviour
{
    void behave(std::weak_ptr<Actor> actor_wp) override
    {
    }
};
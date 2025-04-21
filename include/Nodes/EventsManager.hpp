#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <functional>
#include <typeindex>
#include <vector>
#include "Node.hpp"

struct SFMLEvent
{
    std::function<void(const sf::Event)> callable;
    std::weak_ptr<Node> caller;
    unsigned long u_id;
};

class EventManager : public Node
{
    private:
    std::unordered_map<std::type_index, std::vector<SFMLEvent>> sfml_events;
    unsigned long last_id = 0;
    public:
    template<typename T>
    unsigned long register_sfml_event(std::function<void(const T)> callable, std::weak_ptr<Node> caller);
    void remove_sfml_event(unsigned long event_id);
    void update(float delta) override;
};
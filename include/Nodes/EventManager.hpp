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

class TimedEvent : public Node
{
    private:
    const float time_set;
    float time_elapsed;
    int repetitions;
    std::function<void(void)> callable;
    std::weak_ptr<Node> caller;
    public:
    TimedEvent(float time, std::function<void(void)> callable, 
               std::weak_ptr<Node> caller, int reps);
    void update(float delta) override;
};

class EventManager : public Node
{
    private:
    std::unordered_map<std::type_index, std::vector<SFMLEvent>> sfml_events;
    unsigned long last_id = 0;
    std::vector<std::weak_ptr<TimedEvent>> timed_events;
    
    public:
    template<class T>
    unsigned long register_sfml_event(std::function<void(const T)> callable, std::weak_ptr<Node> caller);
    void remove_sfml_event(unsigned long event_id);

    std::weak_ptr<TimedEvent> register_timed_event(float time, std::function<void(void)> callable, std::weak_ptr<Node> caller = {}, int reps = -1);
    void remove_timed_event(std::weak_ptr<TimedEvent> ev);

    void update(float delta) override;
};

#include "EventManager.ipp"
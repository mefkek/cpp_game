#pragma once
#include "Application.hpp"
#include "ContainerNode.hpp"
#include "Node.hpp"
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <functional>
#include <typeindex>
#include <vector>

class Event
{
    //empty class for storage
};

template<typename... func_args>
class EventBase : public Node, public Event
{
    private:
    struct CallablePair
    {
        unsigned long id;
        std::weak_ptr<Node> caller;
        std::function<void(func_args...)> callable;
        CallablePair(unsigned long id, std::weak_ptr<Node> caller, std::function<void(func_args...)> callable)
            : id{id}, caller{caller}, callable{callable} {};
    };

    protected:
    std::vector<CallablePair> callables;

    public:
    void update(float delta) override {};   //skip

    unsigned long subscribe(std::weak_ptr<Node> caller, std::function<void(func_args...)> func);

    void unsubscribe(unsigned long id);

    void rise(func_args... args);
};

class ITryRise
{
    public:
    virtual void try_rise(const std::optional<sf::Event> event) = 0;
};

template<typename Ev>
class SFMLEvent : public EventBase<Ev>, public ITryRise
{
    private:
    public:
    void try_rise(const std::optional<sf::Event> event) override;
    unsigned long subscribe(std::function<void(Ev)> func);
};

class TimedEvent : public EventBase<>
{
    private:
    const float time_set;
    float time_elapsed;
    int repetitions;

    public:
    TimedEvent(float time, int reps);
    void update(float delta) override;
};

template<typename... func_args>
class CustomEvent : public EventBase<func_args...> {};

class WindowEventManager : public ContainerNode<Event>
{
    private:
    std::vector<std::function<bool(void)>> event_checks;

    public:
    template<typename T>
    std::shared_ptr<SFMLEvent<T>> get_event();
    void update(float delta) override;
};

#include "EventManager.ipp"
#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <functional>
#include <typeindex>
#include <vector>
#include "ContainerNode.hpp"
#include "Node.hpp"

template<typename... func_args>
class EventBase : public Node
{
    public:
    std::weak_ptr<Node> caller;
    std::function<void(func_args...)> callable;
    EventBase(std::function<void(func_args...)> _callable, std::weak_ptr<Node> _caller)
              : callable(_callable), caller(_caller) {};
    void update(float delta) override
    {
        //skip
        return;
    }
};

class SFMLEvent : public EventBase<const sf::Event>
{
    public:
    SFMLEvent(std::function<void(const sf::Event)> _callable, std::weak_ptr<Node> _caller)
              : EventBase(_callable, _caller) {}
};

class TimedEvent : public EventBase<float>
{
    private:
    const float time_set;
    float time_elapsed;
    int repetitions;
    public:
    TimedEvent(float time, std::function<void(float)> callable, 
               int reps, std::weak_ptr<Node> caller);
    void update(float delta) override;
};

class ICustomEvent{};

template<typename... func_args>
class CustomEvent : public EventBase<func_args...>, public ICustomEvent
{
    public:
    void rise(func_args&&... args)
    {
        this->callable(std::forward<func_args>(args)...);
    }
    CustomEvent(std::function<void(func_args...)> _callable, std::weak_ptr<Node> _caller) : EventBase<func_args...>(_callable, _caller) {};
};

class EventManager : public Node
{
    private:
    std::shared_ptr<ContainerNode<SFMLEvent>> sfml_events;
    std::shared_ptr<ContainerNode<TimedEvent>> timed_events;
    std::shared_ptr<ContainerNode<ICustomEvent>> custom_events;
    
    public:
    template<class T>
    std::weak_ptr<SFMLEvent> register_sfml_event(std::function<void(const T&)> callable, std::weak_ptr<Node> caller = {});
    void remove_sfml_event(std::weak_ptr<SFMLEvent> ev);

    std::weak_ptr<TimedEvent> register_timed_event(float time, std::function<void(float)> callable, std::weak_ptr<Node> caller = {}, int reps = -1);
    void remove_timed_event(std::weak_ptr<TimedEvent> ev);

    template<typename... Args>
    std::weak_ptr<CustomEvent<Args...>> register_custom_event(std::function<void(Args...)> callable, std::weak_ptr<Node> caller = {});
    template<typename... Args>
    void remove_custom_event(std::weak_ptr<CustomEvent<Args...>> ev);

    void update(float delta) override;
};

#include "EventManager.ipp"
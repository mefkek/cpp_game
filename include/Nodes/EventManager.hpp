#pragma once
#include "Application.hpp"
#include "ContainerNode.hpp"
#include "Node.hpp"
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <functional>
#include <typeindex>
#include <vector>

template<typename... func_args>
class EventBase : public Node
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
    std::weak_ptr<Node> caller;
    std::vector<CallablePair> callables;

    public:
    void update(float delta) override {};   //skip 

    EventBase(std::weak_ptr<Node> caller) : caller(caller) {}

    unsigned long subscribe(std::weak_ptr<Node> caller, std::function<void(func_args...)> func);

    void unsubscribe(unsigned long id);

    void rise(func_args... args);
};


template<typename Ev>
class SFMLEvent : public EventBase<Ev>
{
    private:
    sf::WindowBase* window;
    public:
    SFMLEvent(sf::WindowBase* window, std::weak_ptr<Node> caller);
    void update(float delta) override;
};

class TimedEvent : public EventBase<>
{
    private:
    const float time_set;
    float time_elapsed;
    int repetitions;

    public:
    TimedEvent(float time, int reps, std::weak_ptr<Node> caller);
    void update(float delta) override;
};

template<typename... func_args>
class CustomEvent : public EventBase<func_args...>
{
    public:
    CustomEvent(std::weak_ptr<Node> caller);
};

class WindowEventManager : public ContainerNode<Node> {};   //this propably should be a container class with all sfml events

template<typename Ev, typename... Args>
std::weak_ptr<Ev> create(std::weak_ptr<Node> caller, Args&&... args);

/*template<typename Ev, typename... Args>
std::weak_ptr<SFMLEvent<Ev>> create_sfml_event(Args&&... args) 
{
    return create<SFMLEvent<Ev>>(Application::instance().get_manager<WindowEventManager>(), std::forward<Args>(args)...);
}*/

#include "EventManager.ipp"
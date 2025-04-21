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

    template<class T>
    unsigned long register_sfml_event(std::function<void(const T)> callable, std::weak_ptr<Node> caller)
    {
        SFMLEvent n_event;
        n_event.callable = [callable](const sf::Event& e){callable(*e.getIf<T>());};
        n_event.caller = caller;
        n_event.u_id = last_id++;
    
        sfml_events[typeid(T)].push_back(n_event);  //typeid becouse event is a variant
        return n_event.u_id;
    }

    void remove_sfml_event(unsigned long event_id);
    void update(float delta) override;
};
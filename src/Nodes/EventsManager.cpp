#include "Nodes/EventsManager.hpp"
#include "Nodes/Application.hpp"
#include <iostream>
#include <algorithm>

template<typename T>
unsigned long EventManager::register_sfml_event(std::function<void(const T)> callable, std::weak_ptr<Node> caller)
{
    SFMLEvent n_event;
    n_event.callable = [callable](const sf::Event& e){callable(std::get<T>(e));};
    n_event.caller = caller;
    n_event.u_id = last_id++;

    sfml_events[typeid(T)].push_back(n_event);
    return n_event.u_id;
}

void EventManager::remove_sfml_event(unsigned long event_id)
{
    for(auto& [event, handlers] : sfml_events)
    {
        auto it = std::find_if(handlers.begin(), handlers.end(),
                               [&](const SFMLEvent o){return o.u_id == event_id;});

        if(it != handlers.end())
        {
            handlers.erase(it);
            return;
        }
    }

    std::cerr << "Event with id: " << event_id << " has not been found.\n";
}

void EventManager::update(float delta)
{
    while(const std::optional event = Application::instance().get_window().pollEvent())
    {
        event->visit([&](const auto& ev_variant)
        {
            using T = std::decay_t<decltype(ev_variant)>;
            auto it = sfml_events.find(typeid(T));
            if(it != sfml_events.end())
            {
                for(const auto& handler : it->second)
                {
                    if(handler.caller.lock())
                    {
                        handler.callable(*event);
                    }
                }
            }
        });
    }
}
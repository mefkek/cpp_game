#include "Nodes/EventsManager.hpp"
#include "Nodes/Application.hpp"
#include "Utility/Logger.hpp"
#include <iostream>
#include <algorithm>

void EventManager::remove_sfml_event(unsigned long event_id)
{
    for(auto& [event, handlers] : sfml_events)
    {
        //find based on unique id, if you need more than 4.294.967.295 events then its beetween god and you
        auto it = std::find_if(handlers.begin(), handlers.end(),
                               [&](const SFMLEvent o){return o.u_id == event_id;});

        if(it != handlers.end())
        {
            handlers.erase(it);
            return;
        }
    }

    Logger::log(Logger::MessageType::Warning, "Sfml event with id: ", event_id, " has not been found.");
}

void EventManager::update(float delta)
{
    /*
        Figure out event type using variant magic, get vector from a map and call if caller still exits
    */
    while(const std::optional event = Application::instance().get_window().pollEvent())
    {
        event->visit([&](const auto& ev_variant)
        {
            using T = std::decay_t<decltype(ev_variant)>;
            auto it = sfml_events.find(typeid(T));
            if(it != sfml_events.end())
            {
                for(auto handler = it->second.begin(); handler != it->second.end();)
                {
                    if(handler->caller.lock())
                    {
                        handler->callable(*event);
                        ++handler;
                    }
                    else
                    {
                        handler = it->second.erase(handler);
                    }
                }
            }
        });
    }
}
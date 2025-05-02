#include "Nodes/EventManager.hpp"
#include "Nodes/Application.hpp"
#include "Utility/Logger.hpp"
#include <iostream>
#include <algorithm>

void EventManager::remove_sfml_event(std::weak_ptr<SFMLEvent> ev)
{
    sfml_events->remove_element(ev);
}

void EventManager::update(float delta)
{
    /*
        Figure out event type using variant magic, get vector from a map and call if caller still exits
    */
    while(const std::optional event = Application::instance().get_window().pollEvent())
    {
        for(auto ev_it = sfml_events->get_children().begin(); ev_it != sfml_events->get_children().end();)
        {
            if(auto ev = std::dynamic_pointer_cast<SFMLEvent>(*ev_it))
            {
                if(!ev->caller.expired())
                {
                    ev->callable(*event);
                    ++ev_it;
                }
                else
                {
                    ev_it = sfml_events->get_children().erase(ev_it);
                }
            }
        }
    }
}

std::weak_ptr<TimedEvent> EventManager::register_timed_event(float time, std::function<void(float)> callable, std::weak_ptr<Node> caller, int reps)
{
    if(!timed_events)
    {
        timed_events = add_child<ContainerNode<TimedEvent>>().lock();
    }
    if(caller.expired())
    {
        caller = shared_from_this();
    }

    return timed_events->add_element(time, callable, reps, caller);
}

void EventManager::remove_timed_event(std::weak_ptr<TimedEvent> ev)
{
    timed_events->remove_element(ev);
}
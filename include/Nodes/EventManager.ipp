template<class T>
std::weak_ptr<SFMLEvent> EventManager::register_sfml_event(std::function<void(const T&)> callable,
                                                           std::weak_ptr<Node> caller)
{
    if(!sfml_events)
    {
        sfml_events = add_child<ContainerNode<SFMLEvent>>().lock();
    }
    if(caller.expired())
    {
        caller = shared_from_this();
    }

    return sfml_events->add_element([callable](const sf::Event& e)
                                            {
                                                if(auto event = e.getIf<T>())
                                                {
                                                    callable(*event);
                                                }
                                            }, caller);
}

template<typename... Args>
std::weak_ptr<CustomEvent<Args...>> EventManager::register_custom_event(std::function<void(Args...)> callable,
                                    std::weak_ptr<Node> caller)
{
    if(!custom_events)
    {
        custom_events = add_child<ContainerNode<ICustomEvent>>().lock();
    }
    if(caller.expired())
    {
        caller = shared_from_this();
    }

    return custom_events->add_child<CustomEvent<Args...>>(callable, caller);
}

template<typename... Args>
void EventManager::remove_custom_event(std::weak_ptr<CustomEvent<Args...>> ev)
{
    custom_events->remove_child(ev);
}
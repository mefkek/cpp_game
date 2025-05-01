template<class T>
unsigned long EventManager::register_sfml_event(std::function<void(const T)> callable, std::weak_ptr<Node> caller)
{
    SFMLEvent n_event;
    n_event.callable = [callable](const sf::Event& e){callable(*e.getIf<T>());};
    n_event.caller = caller;
    n_event.u_id = last_id++;

    sfml_events[typeid(T)].push_back(n_event);  //typeid becouse event is a variant
    return n_event.u_id;
}
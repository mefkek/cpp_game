template<typename T>
constexpr bool IsSFMLEvent = std::is_same_v<T, sf::Event::Closed> ||
                             std::is_same_v<T, sf::Event::Resized> ||
                             std::is_same_v<T, sf::Event::FocusLost> ||
                             std::is_same_v<T, sf::Event::FocusGained> ||
                             std::is_same_v<T, sf::Event::TextEntered> ||
                             std::is_same_v<T, sf::Event::KeyPressed> ||
                             std::is_same_v<T, sf::Event::KeyReleased> ||
                             std::is_same_v<T, sf::Event::MouseWheelScrolled> ||
                             std::is_same_v<T, sf::Event::MouseButtonPressed> ||
                             std::is_same_v<T, sf::Event::MouseButtonReleased> ||
                             std::is_same_v<T, sf::Event::MouseMoved> ||
                             std::is_same_v<T, sf::Event::MouseMovedRaw> ||
                             std::is_same_v<T, sf::Event::MouseEntered> ||
                             std::is_same_v<T, sf::Event::MouseLeft> ||
                             std::is_same_v<T, sf::Event::JoystickButtonPressed> ||
                             std::is_same_v<T, sf::Event::JoystickButtonReleased> ||
                             std::is_same_v<T, sf::Event::JoystickMoved> ||
                             std::is_same_v<T, sf::Event::JoystickConnected> ||
                             std::is_same_v<T, sf::Event::JoystickDisconnected> ||
                             std::is_same_v<T, sf::Event::TouchBegan> ||
                             std::is_same_v<T, sf::Event::TouchMoved> ||
                             std::is_same_v<T, sf::Event::TouchEnded> ||
                             std::is_same_v<T, sf::Event::SensorChanged>;

template<typename... func_args>
unsigned long EventBase<func_args...>::subscribe(std::weak_ptr<Node> caller, std::function<void(func_args...)> func)
{
    unsigned long prev_id = callables.empty() ? 0 : callables.back().id + 1;
    callables.emplace_back(prev_id, caller, func);
    return prev_id;
}

template<typename... func_args>
void EventBase<func_args...>::unsubscribe(unsigned long id)
{
    callables.erase(std::remove_if(callables.begin(),[&id](const CallablePair& o) {return o.id == id;}),
                    callables.end());
}

template<typename... func_args>
void EventBase<func_args...>::rise(func_args... args)
{
    for(auto ev_it = callables.begin(); ev_it != callables.end(); )
    {
        if(!ev_it->caller.expired())
        {
            ev_it->callable(std::forward<func_args>(args)...);
            ++ev_it;
        }
        else
        {
            ev_it = callables.erase(ev_it);
        }
    }
}

template<typename Ev>
void SFMLEvent<Ev>::try_rise(const std::optional<sf::Event> event)
{
    if(auto e = event->getIf<Ev>())
    {
        this->rise(*e);
    }
}

template<typename Ev>
unsigned long SFMLEvent<Ev>::subscribe(std::function<void(Ev)> func)
{
    return EventBase<Ev>::subscribe(this->shared_from_this(), func);
}

template<typename Ev, typename... Args>
std::weak_ptr<Ev> create(std::weak_ptr<Node> caller, Args&&... args) 
{
    return caller.lock()->add_child<Ev>(std::forward<Args>(args)..., caller);
}

template<typename T>
std::shared_ptr<SFMLEvent<T>> WindowEventManager::get_event()
{
    static_assert(IsSFMLEvent<T>, "WindowEventManager::get_event type must be an sf::Event");

    for(auto& c : children)
    {
        if(auto p = std::dynamic_pointer_cast<SFMLEvent<T>>(c))
        {
            return p;
        }
    }

    return this->add_child<SFMLEvent<T>>().lock();  //should't fail but sketchy either way
}
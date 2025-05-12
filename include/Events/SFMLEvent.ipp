template<typename T>
//basically a check if type is an event used in WindowEventManager
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
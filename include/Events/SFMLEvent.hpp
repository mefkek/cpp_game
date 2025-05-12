#pragma once
#include "EventBase.hpp"
#include <SFML/Graphics.hpp>


class ITryRise
{
    public:
    virtual void try_rise(const std::optional<sf::Event> event) = 0;
};

template<typename Ev>
class SFMLEvent : public EventBase<Ev>, public ITryRise
{
    private:
    public:
    void try_rise(const std::optional<sf::Event> event) override;
    unsigned long subscribe(std::function<void(Ev)> func);
};

#include "SFMLEvent.ipp"
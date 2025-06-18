#pragma once
#include "SFMLEvent.hpp"
#include "Nodes/ContainerNode.hpp"
#include <functional>
#include <vector>

class WindowEventManager : public ContainerNode<Event>
{
    private:
    std::vector<std::function<bool(void)>> event_checks;


    public:
    template<typename T>
    std::shared_ptr<SFMLEvent<T>> get_event();
    void update(float delta) override;
};

#include "WindowEventManager.ipp"
#pragma once
#include "Nodes/Node.hpp"
#include <functional>
#include <algorithm>
#include <vector>

class Event
{
    //empty class for storage
};

template<typename... func_args>
class EventBase : public Node, public Event
{
    private:
    struct CallablePair
    {
        unsigned long id;
        std::weak_ptr<Node> caller;
        std::function<void(func_args...)> callable;
        CallablePair(unsigned long id, std::weak_ptr<Node> caller, std::function<void(func_args...)> callable)
            : id{id}, caller{caller}, callable{callable} {};
    };

    protected:
    std::vector<CallablePair> callables;

    public:
    void update(float delta) override {};   //skip

    unsigned long subscribe(std::weak_ptr<Node> caller, std::function<void(func_args...)> func);

    void unsubscribe(unsigned long id);

    void rise(func_args... args);
};

#include "EventBase.ipp"
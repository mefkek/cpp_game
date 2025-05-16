#include "Nodes/Colliders.hpp"
#include "Nodes/TriggerArea.hpp"
#include "Utility/Logger.hpp"
#include <algorithm>

TriggerArea::TriggerArea(sf::Vector2f position, sf::Vector2f size) : position(position), size(size), doCircle(false) {}

TriggerArea::TriggerArea(sf::Vector2f position, float radius)
                         : position(position), size({radius*2, radius*2}), doCircle(true) {}

TriggerAreaColliderCircle::TriggerAreaColliderCircle(sf::Vector2f position, float size)
                                                                : CircleCollider(position, size) {}

void TriggerAreaColliderCircle::on_collision(const std::weak_ptr<Collider> other)
{
    Logger::log(Logger::MessageType::Info, shared_from_this(), " collision detected");
    entered_frame.push_back(other);
}

void TriggerAreaColliderCircle::update(float delta)
{
    std::vector<std::shared_ptr<Collider>> previous;
    std::vector<std::shared_ptr<Collider>> current;

    std::vector<std::shared_ptr<Collider>> new_colliders;
    std::vector<std::shared_ptr<Collider>> left;

    //filtering expired pointers
    for(auto n : entered_frame)
    {
        if(auto p = n.lock())
        {
            current.push_back(p);
        }
    }

    for(auto old : entered)
    {
        if(auto p = old.lock())
        {
            previous.push_back(p);
        }
    }

    //finding new entries and colliders that left the area
    for (auto& ptr : current)
    {
        auto it = std::find_if(previous.begin(), previous.end(),
                               [&ptr](const std::shared_ptr<Collider>& old) {
                                   return old == ptr;
                               });
        if (it == previous.end())
        {
            new_colliders.push_back(ptr);
        }
    }

    for (auto& ptr : previous)
    {
        auto it = std::find_if(current.begin(), current.end(),
                               [&ptr](const std::shared_ptr<Collider>& now) {
                                   return now == ptr;
                               });
        if (it == current.end())
        {
            left.push_back(ptr);
        }
    }

    //rising events
    if (auto p = std::dynamic_pointer_cast<TriggerArea>(parent.lock()))
    {
        if (auto entered_ev = p->get_on_entered_event().lock())
        {
            for (auto& ent : new_colliders)
            {
                entered_ev->rise(ent);
            }
        }
        if (auto exited_ev = p->get_on_exit_event().lock())
        {
            for (auto& ex : left)
            {
                exited_ev->rise(ex);
            }
        }
    }
    else
    {
        Logger::log(Logger::MessageType::Warning, shared_from_this(), "Invalid parent");
    }

    //cleanup
    entered.clear();
    for (auto& e : current)
    {
        entered.push_back(e);
    }

    entered_frame.clear();
}

TriggerAreaColliderRect::TriggerAreaColliderRect(sf::Vector2f position, sf::Vector2f size)
                                                                : RectangleCollider(position, size) {}

void TriggerAreaColliderRect::on_collision(const std::weak_ptr<Collider> other)
{
    Logger::log(Logger::MessageType::Info, shared_from_this(), " collision detected");
    entered_frame.push_back(other);
}

void TriggerAreaColliderRect::update(float delta)
{
    std::vector<std::shared_ptr<Collider>> previous;
    std::vector<std::shared_ptr<Collider>> current;

    std::vector<std::shared_ptr<Collider>> new_colliders;
    std::vector<std::shared_ptr<Collider>> left;

    //filtering expired pointers
    for(auto n : entered_frame)
    {
        if(auto p = n.lock())
        {
            current.push_back(p);
        }
    }

    for(auto old : entered)
    {
        if(auto p = old.lock())
        {
            previous.push_back(p);
        }
    }

    //finding new entries and colliders that left the area
    for (auto& ptr : current)
    {
        auto it = std::find_if(previous.begin(), previous.end(),
                               [&ptr](const std::shared_ptr<Collider>& old) {
                                   return old == ptr;
                               });
        if (it == previous.end())
        {
            new_colliders.push_back(ptr);
        }
    }

    for (auto& ptr : previous)
    {
        auto it = std::find_if(current.begin(), current.end(),
                               [&ptr](const std::shared_ptr<Collider>& now) {
                                   return now == ptr;
                               });
        if (it == current.end())
        {
            left.push_back(ptr);
        }
    }

    //rising events
    if (auto p = std::dynamic_pointer_cast<TriggerArea>(parent.lock()))
    {
        if (auto entered_ev = p->get_on_entered_event().lock())
        {
            for (auto& ent : new_colliders)
            {
                entered_ev->rise(ent);
            }
        }
        if (auto exited_ev = p->get_on_exit_event().lock())
        {
            for (auto& ex : left)
            {
                exited_ev->rise(ex);
            }
        }
    }
    else
    {
        Logger::log(Logger::MessageType::Warning, shared_from_this(), "Invalid parent");
    }

    //cleanup
    entered.clear();
    for (auto& e : current)
    {
        entered.push_back(e);
    }

    entered_frame.clear();
}

void TriggerArea::initialize()
{
    if(doCircle)
    {
        collider = add_child<TriggerAreaColliderCircle>(position, size.x/2);
    }
    else
    {
        collider = add_child<TriggerAreaColliderRect>(position, size);
    }

    on_entered_ev = add_child<CustomEvent<const std::weak_ptr<Collider>>>();
    on_entered_ev.lock()->subscribe(shared_from_this(), [this](const std::weak_ptr<Collider> other)
                                                        {
                                                            this->on_entered(other);
                                                        });

    on_exit_ev = add_child<CustomEvent<const std::weak_ptr<Collider>>>();
    on_exit_ev.lock()->subscribe(shared_from_this(), [this](const std::weak_ptr<Collider> other)
                                                        {
                                                            this->on_exit(other);
                                                        });
}

void TriggerArea::update(float delta)
{
    collider.lock()->position = this->position;
}

std::weak_ptr<CustomEvent<const std::weak_ptr<Collider>>> TriggerArea::get_on_entered_event()
{
    return on_entered_ev;
}
std::weak_ptr<CustomEvent<const std::weak_ptr<Collider>>> TriggerArea::get_on_exit_event()
{
    return on_exit_ev;
}
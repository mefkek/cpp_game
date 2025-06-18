#pragma once

#include <SFML/Graphics.hpp>
#include "Nodes/ContainerNode.hpp"
#include "Nodes/Actor.hpp"

// A Party is a container node for Actor instances.
// Child management and update propagation are handled by ContainerNode.
class Party : public ContainerNode<Actor>
{
protected:
    using ContainerNode<Actor>::add_element;

public:
    template<typename... Args>
    std::shared_ptr<Actor> add_actor(Args&&... args)
    {
        int counter = 0;
        for(auto ch : children)
        {
            if(std::dynamic_pointer_cast<Actor>(ch))
            {
                counter++;
                if(counter > 5)
                {
                    Logger::log(Logger::MessageType::Warning, "Tried adding more actors than is allowed"
                                "to party: ", std::static_pointer_cast<Node>(shared_from_this()));
                    return nullptr;
                }
            }
        }

        auto a_ptr = add_element(std::forward<Args>(args)...);

        return a_ptr.lock();
    }

    void display(sf::Vector2f position, sf::Vector2f scale, sf::Vector2f sp_scale, bool do_cards = false);
    void display_cards();
};

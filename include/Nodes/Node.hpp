#pragma once
#include "Utility/Logger.hpp"
#include <iostream>
#include <vector>
#include <memory>

class Node : public std::enable_shared_from_this<Node>
{
    /*
        Base class for all things that should be updated each frame,
        Also acts as a base for tree structre
    */
    protected:
    std::vector<std::shared_ptr<Node>> children;
    std::weak_ptr<Node> parent;

    public:
    Node() = default;   //should only be used when adding root (friend Application?)

    virtual void update(float) = 0;

    std::vector<std::shared_ptr<Node>>& get_children();

    template <typename T, typename ... Args>
    std::weak_ptr<T> add_child(Args&&... args)
    {
        std::shared_ptr<T> new_child = std::make_shared<T>(std::forward<Args>(args)...);
        if(auto p = std::dynamic_pointer_cast<Node>(new_child))     //if child is not a node, ignore
        {
            p->parent = shared_from_this();
            children.push_back(new_child);
            return new_child;
        }

        Logger::log(Logger::MessageType::Warning, "Node ", this->shared_from_this(), " tried to add invalid child type.");
        return {};
    }

    void remove_child(std::shared_ptr<Node>);

    void kill();

    virtual ~Node() = default;
};

std::ostream& operator<<(std::ostream& os, const std::shared_ptr<Node>& n);
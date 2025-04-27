#pragma once
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
    Node() = default;

    virtual void update(float) = 0;

    std::vector<std::shared_ptr<Node>>& get_children();

    template <typename T, typename ... Args>
    void add_child(Args&&... args)
    {
        std::shared_ptr<T> new_child = std::make_shared<T>(std::forward<Args>(args)...);
        if(auto p = std::dynamic_pointer_cast<Node>(new_child))
        {
            p->parent = shared_from_this();
            children.push_back(new_child);
            return;
        }

        std::cerr << "Tried to add invalid child type.\n";
    }

    void remove_child(std::shared_ptr<Node>);

    void kill();

    virtual ~Node() = default;
};
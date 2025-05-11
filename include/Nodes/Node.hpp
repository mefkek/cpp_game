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

    Node() = default;   //should only be used when adding root (friend Application?)

    public:
    virtual void initialize() {}

    virtual void update(float) = 0;

    std::vector<std::shared_ptr<Node>>& get_children();

    template <typename T, typename ... Args>
    void add_child(Args&&... args);

    void remove_child(std::shared_ptr<Node>);

    void kill();

    virtual ~Node() = default;
};

std::ostream& operator<<(std::ostream& os, const std::shared_ptr<Node>& n);

template<typename T, typename... Args>
std::shared_ptr<T> create(Args&&... args);

#include "Node.ipp"
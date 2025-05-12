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
    virtual void initialize() {}    //acts as a contructor without the danger of bad_weak_ptr

    virtual void update(float) = 0;

    std::vector<std::shared_ptr<Node>>& get_children();

    template <typename T, typename ... Args>
    void add_child(Args&&... args);

    void remove_child(std::shared_ptr<Node>);

    void kill();

    virtual ~Node() = default;
};

std::ostream& operator<<(std::ostream& os, std::shared_ptr<Node>& n);

/*
    this should be used any time a node is added, but
    as a contructor needs to be public it can't be enforced
*/
template<typename T, typename... Args>
std::shared_ptr<T> create(Args&&... args);

#include "Node.ipp"
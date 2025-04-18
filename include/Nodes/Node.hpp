#pragma once
#include <vector>
#include <memory>

class Node
{
    /*
        Base class for all things that should be updated each frame,
        Also acts as a base for tree structre
    */
    protected:
    std::vector<std::shared_ptr<Node>> children;
    std::shared_ptr<Node> parent;
    public:
    Node() = default;
    virtual void update(float) = 0;
    std::vector<std::shared_ptr<Node>>& get_children();
    void add_child(std::shared_ptr<Node>);
    virtual ~Node();
};
#include "Nodes/Node.hpp"
#include <iostream>
#include <algorithm>

std::vector<std::shared_ptr<Node>>& Node::get_children()
{
    return children;
}

void Node::remove_child(std::shared_ptr<Node> ch)
{
    children.erase(std::remove(children.begin(), children.end(), ch), children.end());
}

void Node::kill()
{
    if(auto p = this->parent.lock())
    {
        std::vector<std::shared_ptr<Node>>& p_children = p->get_children();
        p_children.erase(std::remove(p_children.begin(), p_children.end(), shared_from_this()), p_children.end());
        return;
    }

    std::cerr << "Tried removing parentless node (may be root).\n";
}
#include "Nodes/Node.hpp"
#include <iostream>

void Node::add_child(std::shared_ptr<Node> ch)
{
    children.push_back(ch);
}

std::vector<std::shared_ptr<Node>>& Node::get_children()
{
    return children;
}

Node::~Node()
{
    children.clear();
}
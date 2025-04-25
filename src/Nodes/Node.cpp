#include "Nodes/Node.hpp"
#include <iostream>
#include <algorithm>

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

void Node::remove_child(std::shared_ptr<Node> ch)
{
    children.erase(std::remove(children.begin(), children.end(), ch), children.end());
}

void Node::kill()
{
    std::vector<std::shared_ptr<Node>>& p_children = this->parent->get_children();
    p_children.erase(std::remove(p_children.begin(), p_children.end(), shared_from_this()), p_children.end());
}
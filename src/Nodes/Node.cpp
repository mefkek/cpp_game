#include "Utility/Various.hpp"
#include "Nodes/Node.hpp"
#include <iostream>
#include <typeindex>
#include <algorithm>

/*
    For Node name demangling on GCC/Clang, MSVC should have it human readable
    Source: https://stackoverflow.com/questions/281818/unmangling-the-result-of-stdtype-infoname
*/

std::vector<std::shared_ptr<Node>>& Node::get_children()
{
    return children;
}

void Node::remove_child(std::shared_ptr<Node> ch)
{
    size_t size_b = children.size();
    children.erase(std::remove(children.begin(), children.end(), ch), children.end());

    if(size_b != children.size())
    {
        Logger::log(Logger::MessageType::Warning, "Tried removind node: ", ch, "which is not a child of", shared_from_this(), ".");
    }
}

void Node::kill()
{
    /*
        node removes itslef from parents children vector, wont work if
        orphaned (if nodes are orphaned then its bad, but shared_ptr
        should help with that) or its a root node (which is good)
    */

    if(auto p = this->parent.lock())
    {
        std::vector<std::shared_ptr<Node>>& p_children = p->get_children();
        p_children.erase(std::remove(p_children.begin(), p_children.end(), shared_from_this()), p_children.end());
        return;
    }

    Logger::log(Logger::MessageType::Warning,
                "Tried removing parentless node ", this->shared_from_this(), " (may be root).");
}

std::ostream& operator<<(std::ostream& os, const std::shared_ptr<Node>& n)
{

    // prints something like FPSCounter0x9309e50
    // 0x9... is a memory address that doubles as node id in that case
    os << demangle(std::type_index(typeid(*n)).name()) << n.get();

    return os;
}
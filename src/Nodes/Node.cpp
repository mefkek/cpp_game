#include "Nodes/Node.hpp"
#include <iostream>
#include <typeindex>
#include <algorithm>

/*
    For Node name demangling on GCC/Clang, MSVC should have it human readable
    Source: https://stackoverflow.com/questions/281818/unmangling-the-result-of-stdtype-infoname
*/
#ifdef __GNUG__
#include <cxxabi.h>

std::string demangle(const char* name) {
    int status = -1;
    std::unique_ptr<char[], void(*)(void*)> result(
        abi::__cxa_demangle(name, nullptr, nullptr, &status),
        std::free
    );
    return (status == 0) ? result.get() : name;
}
#endif

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

    std::cerr << "Tried removing parentless node (may be root).\n";
}

std::ostream& operator<<(std::ostream& os, std::shared_ptr<Node>& n)
{

    // prints something like FPSCounter0x9309e50
    // 0x9... is a memory address that doubles as node id in that case
    #ifdef __GNUG__
    os << demangle(std::type_index(typeid(*n)).name()) << n.get();
    #else
    os << std::type_index(typeid(*n)).name() << n.get();
    #endif

    return os;
}
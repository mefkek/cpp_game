#include "Utility/Various.hpp"
#include "Nodes/Application.hpp"
#include <sstream>
#include <queue>

std::string demangle(const char* name)
{
    #ifdef __GNUG__
    int status = -1;
    std::unique_ptr<char[], void(*)(void*)> result(
        abi::__cxa_demangle(name, nullptr, nullptr, &status),
        std::free
    );
    return (status == 0) ? result.get() : name;
    #else
    return name;
    #endif
}

std::string print_tree()
{
    std::stringstream res;
    res << "Printing current tree structure:\n";
    struct Visited
    {
        const std::shared_ptr<Node> ptr;
        unsigned int level;
    };

    std::queue<Visited> q;
    for(const auto& node : Application::instance().get_root_level())
    {
        q.push({node, 0});
    }

    while(!q.empty())
    {
        Visited current = q.front();
        q.pop();

        res << std::string(current.level * 2, ' ') << (current.level > 0 ? ">>" : "") << current.ptr << '\n';

        for(const auto& ch : current.ptr->get_children())
        {
            q.push({ch, current.level + 1});
        }
    }

    std::string str = res.str();
    if(!str.empty())
    {
        str.pop_back();
    }

    return str;
}
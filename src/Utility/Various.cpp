#include "Utility/Various.hpp"

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
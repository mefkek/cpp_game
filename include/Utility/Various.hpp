#pragma once
#include <iostream>
#include <memory>

#ifdef __GNUG__
#include <cxxabi.h>
#endif

/*
    For Node name demangling on GCC/Clang, MSVC should have it human readable
    Source: https://stackoverflow.com/questions/281818/unmangling-the-result-of-stdtype-infoname
*/
std::string demangle(const char* name);

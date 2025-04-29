#pragma once
#include <iostream>
#include <memory>

#ifdef __GNUG__
#include <cxxabi.h>
#endif

std::string demangle(const char* name);

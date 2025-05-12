#pragma once
#include "EventBase.hpp"

template<typename... func_args>
class CustomEvent : public EventBase<func_args...> {};
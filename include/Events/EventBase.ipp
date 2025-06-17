template<typename... func_args>
unsigned long EventBase<func_args...>::subscribe(std::weak_ptr<Node> caller, std::function<void(func_args...)> func)
{
    unsigned long prev_id = callables.empty() ? 0 : callables.back().id + 1;
    callables.emplace_back(prev_id, caller, func);
    return prev_id;
}

template<typename... func_args>
void EventBase<func_args...>::unsubscribe(unsigned long id)
{
    callables.erase(std::remove_if(callables.begin(), callables.end(),
                    [&id](const CallablePair& o) {return o.id == id;}),
                    callables.end());
}

template<typename... func_args>
void EventBase<func_args...>::rise(func_args... args)
{
    callables.erase(
        std::remove_if(callables.begin(), callables.end(),
        [](const CallablePair& o) { return o.caller.expired(); }),
        callables.end());

    for(auto& callable : callables)
    {
        if (callable.callable)
            callable.callable(std::forward<func_args>(args)...);
    }
}
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
    callables.erase(std::remove_if(callables.begin(),[&id](const CallablePair& o) {return o.id == id;}),
                    callables.end());
}

template<typename... func_args>
void EventBase<func_args...>::rise(func_args... args)
{
    for(auto ev_it = callables.begin(); ev_it != callables.end(); )
    {
        if(!ev_it->caller.expired())
        {
            ev_it->callable(std::forward<func_args>(args)...);
            ++ev_it;
        }
        else
        {
            ev_it = callables.erase(ev_it);
        }
    }
}
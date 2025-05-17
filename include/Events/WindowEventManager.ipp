template<typename Ev, typename... Args>
std::weak_ptr<Ev> create(std::weak_ptr<Node> caller, Args&&... args) 
{
    return caller.lock()->add_child<Ev>(std::forward<Args>(args)..., caller);
}

template<typename T>
std::shared_ptr<SFMLEvent<T>> WindowEventManager::get_event()
{
    static_assert(IsSFMLEvent<T>, "WindowEventManager::get_event type must be an sf::Event");

    for(auto& c : children)
    {
        if(auto p = std::dynamic_pointer_cast<SFMLEvent<T>>(c))
        {
            return p;
        }
    }

    //return this->add_child<SFMLEvent<T>>().lock();  //should't fail but sketchy either way
    // add the new SFMLEvent<T> as a child
    add_child<SFMLEvent<T>>();

    // grab it back out of children[]
    auto& kids = this->get_children();
    return std::dynamic_pointer_cast<SFMLEvent<T>>(kids.back());

}
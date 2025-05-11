template<typename T, typename... Args>
std::shared_ptr<T> create(Args&&... args)
{
    static_assert(std::is_base_of_v<Node, T>, "create<T> can only be used with classes derived from Node");

    std::shared_ptr<T> new_node = std::make_shared<T>(std::forward<Args>(args)...);
    new_node->initialize();

    return new_node;
}

template <typename T, typename ... Args>
void Node::add_child(Args&&... args)
{
    children.emplace_back(create<T, Args...>(std::forward<Args>(args)...));
}
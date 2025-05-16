#include <typeindex>
#include "Utility/Various.hpp"

template<typename T>
void ContainerNode<T>::update(float delta)
{
    //skip
    return;
}

template<typename T>
template<typename... Args>
std::weak_ptr<T> ContainerNode<T>::add_element(Args&&... args)
{
    return this->add_child<T>(std::forward<Args>(args)...);
}

template<typename T>
std::weak_ptr<T> ContainerNode<T>::find_element(std::weak_ptr<T> el)
{
    return std::find_if(children.begin(), children.end(),
                        [el](std::shared_ptr<Node> o)
                        {
                            if(auto p = std::dynamic_pointer_cast<T>(o))
                            {
                                return !el.expired() && o == el.lock();
                            }
                            return false;
                        });
}

template<typename T>
void ContainerNode<T>::remove_element(std::weak_ptr<T> el)
{
    if(auto locked = el.lock())
    {
        if(auto p = std::dynamic_pointer_cast<T>(locked))
        {
            remove_child(p);
            return;
        }
    }
    Logger::log(Logger::MessageType::Warning, "Tried to remove an expired pointer in container: ",
                    shared_from_this());
}
#pragma once
#include <algorithm>
#include "Node.hpp"

template<typename T>
/*
    Class template for Nodes with container functionality
    Children of type other than T can still be added,
    but wonn't be taken into account when using specific functions
*/
class ContainerNode : public Node
{
    public:
    void update(float delta) override;
    template<typename... Args>
    std::weak_ptr<T> add_element(Args&&... args);
    std::weak_ptr<T> find_element(std::weak_ptr<T> el);
    void remove_element(std::weak_ptr<T> el);
};

#include "ContainerNode.ipp"
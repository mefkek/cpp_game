#pragma once
#include "Nodes/ContainerNode.hpp"
#include "Label.hpp"
#include <functional>

class SelectableLabel : public Label
{
    public:
    std::function<void()> func;
    SelectableLabel(std::function<void()> on_selected, const std::string layer_name, const sf::Font& font, const std::string& content = "", unsigned int size = 30);
};

class Menu : public ContainerNode<SelectableLabel>, public sf::Transformable
{
    private:
    std::vector<std::weak_ptr<SelectableLabel>> menu_items;
    int current_selection;
    unsigned long ev_id;
    protected:
    using ContainerNode<SelectableLabel>::add_element;
    bool ready = false;
    public:
    bool active = true;
    void initialize() override;
    std::weak_ptr<SelectableLabel> add_item(const SelectableLabel item);
    void update(float delta) override;
    ~Menu();
};
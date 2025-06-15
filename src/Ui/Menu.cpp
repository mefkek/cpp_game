#include "Ui/Menu.hpp"
#include "Events/WindowEventManager.hpp"
#include "Nodes/Application.hpp"

SelectableLabel::SelectableLabel(std::function<void()> on_selected, const std::string layer_name, const sf::Font& font, const std::string& content, unsigned int size)
    : Label(layer_name, font, content, size)
{
    func = on_selected;
}

void Menu::initialize()
{
    ev_id = Application::instance().get_manager<WindowEventManager>()->get_event<sf::Event::KeyPressed>()->
        subscribe([&](const sf::Event::KeyPressed& e)
        {
            if(!active)
            {
                return;
            }
            
            if(e.scancode == sf::Keyboard::Scancode::Enter)
            {
                if(auto ptr = menu_items[current_selection].lock())
                {
                    ptr->func();
                }
            }
            else if(e.scancode == sf::Keyboard::Scancode::Up)
            {
                ++current_selection;
            }
            else if(e.scancode == sf::Keyboard::Scancode::Up)
            {
                --current_selection;
            }

            if(current_selection < 0)
            {
                current_selection = menu_items.size() - 1;
            }
            if(current_selection >= menu_items.size())
            {
                current_selection = 0;
            }
        });
}

void Menu::add_item(const SelectableLabel item)
{
    std::shared_ptr<SelectableLabel> ptr = std::make_shared<SelectableLabel>(item);
    menu_items.push_back(ptr);
    ContainerNode<SelectableLabel>::add_element(ptr);
}

void Menu::update(float delta)
{
    for(auto it = menu_items.begin(); it != menu_items.end();)
    {
        auto ptr = *it;
        if(auto p = ptr.lock())
        {
            p->setPosition(getPosition());
            auto prev_it = --it;
            if(prev_it >= menu_items.begin())
            {
                if(auto prev_ptr = (*prev_it).lock())
                {
                    p->move({prev_ptr->getGlobalBounds().size.x + 5.f, 0.f});
                }
            }
            ++it;
        }
        else
        {
            it = menu_items.erase(it);
        }
    }
}

Menu::~Menu()
{
    Application::instance().get_manager<WindowEventManager>()->get_event<sf::Event::KeyPressed>()->unsubscribe(ev_id);
}
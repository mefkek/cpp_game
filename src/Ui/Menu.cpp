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
    Application::instance().get_manager<WindowEventManager>()->get_event<sf::Event::KeyReleased>()->subscribe(
    shared_from_this(),
    [&](const sf::Event::KeyReleased& e)
    {
        if(e.scancode == sf::Keyboard::Scancode::Enter)
        {
            if(e.scancode == sf::Keyboard::Scancode::Enter)
            {
                if(auto ptr = menu_items[current_selection].lock())
                {
                    ptr->func();
                    return;
                }
            }          
        }
    });

    Application::instance().get_manager<WindowEventManager>()->get_event<sf::Event::KeyPressed>()->
        subscribe(shared_from_this(), [&](const sf::Event::KeyPressed& e)
        {
            if(e.scancode == sf::Keyboard::Scancode::Escape)
            {
                return; //protection agains accesing invalid ptr
            }
            if(!active)
            {
                return;
            }
            
            if(auto ptr = menu_items[current_selection].lock())
            {
                ptr->setScale({1.f, 1.f});
                ptr->setFillColor(sf::Color::White);
            }

            if(e.scancode == sf::Keyboard::Scancode::Up)
            {
                --current_selection;
            }
            else if(e.scancode == sf::Keyboard::Scancode::Down)
            {
                ++current_selection;
            }

            if(current_selection < 0)
            {
                current_selection = menu_items.size() - 1;
            }
            if(current_selection >= menu_items.size())
            {
                current_selection = 0;
            }

            if(auto ptr = menu_items[current_selection].lock())
            {
                ptr->setScale({1.2, 1.2});
                ptr->setFillColor(sf::Color::Cyan);
            }
        });
}

std::weak_ptr<SelectableLabel> Menu::add_item(const SelectableLabel item)
{
    auto ret = ContainerNode<SelectableLabel>::add_element(item);
    menu_items.push_back(ret);
    return ret;
}

void Menu::update(float delta)
{
    if(auto ptr = menu_items[current_selection].lock())
    {
        ptr->setScale({1.2, 1.2});
        ptr->setFillColor(sf::Color::Cyan);
    }
    sf::Vector2f current_pos = getPosition();

    for (auto it = menu_items.begin(); it != menu_items.end();)
    {
        if (auto p = it->lock())
        {
            p->setPosition(current_pos);

            current_pos.y += p->getGlobalBounds().size.y + 20.f;

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
    if(Application::instance().get_window().isOpen())
    {
        //Application::instance().get_manager<WindowEventManager>()->get_event<sf::Event::KeyPressed>()->unsubscribe(ev_id);
    }
}
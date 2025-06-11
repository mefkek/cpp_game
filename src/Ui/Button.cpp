#include "../../include/Ui/Button.hpp"
#include "../../include/Ui/ButtonColliders.hpp"
#include "../../include/Ui/MouseCollider.hpp"

Button::Button(const std::string layer_name, const sf::Font& font, const sf::Texture& texture, sf::Vector2f size,
               func on_pressed, func on_mouse_entered, func on_mouse_left,
               const std::string& content, unsigned int font_size, sf::IntRect rect)
    : layer_name(layer_name), size(size), on_pressed_func(on_pressed), on_mouse_entered_func(on_mouse_entered), on_mouse_left_func(on_mouse_left)
{
    sf::Vector2f translated = Application::instance().get_manager<RenderManager>()->translate_to_layer(layer_name, size);
    ico = create<Icon>(layer_name, texture, rect);
    label = create<Label>(layer_name, font, content, font_size);

    sf::FloatRect bounds = ico->getLocalBounds();
    sf::Vector2f scale = {translated.x / bounds.size.x, translated.y / bounds.size.y};

    ico->setScale(scale);
}

void Button::initialize()
{
    coll = add_child<ButtonCollider>(size, getPosition()).lock();
    trig = add_child<ButtonTriggerArea>(size, getPosition()).lock();
    coll->position = getPosition();
    trig->position = getPosition();

    auto collisions = Application::instance().get_manager<CollisionManager>();
    collisions->add_collider("Debug_coll_coll", coll);
    collisions->add_collider("Debug_coll_trig", trig);

    coll->get_on_collision_event().lock()->subscribe(shared_from_this(), [this](const std::weak_ptr<Collider> other){this->on_pressed(other);});
    trig->get_on_entered_event().lock()->subscribe(shared_from_this(), [this](const std::weak_ptr<Collider> other){this->on_mouse_entered(other);});
    trig->get_on_exit_event().lock()->subscribe(shared_from_this(), [this](const std::weak_ptr<Collider> other){this->on_mouse_left(other);});
    setOrigin(size / 2.f);
}

void Button::update(float delta)
{
    sf::Vector2f translated = Application::instance().get_manager<RenderManager>()->translate_to_layer(layer_name, getPosition());
    ico->setOrigin(ico->getLocalBounds().size / 2.f);
    ico->setPosition(translated);
    label->setOrigin(label->getLocalBounds().size / 2.f);
    label->setPosition(translated);
    coll->position = getPosition();
    trig->position = getPosition();
}

void Button::on_pressed(const std::weak_ptr<Collider> other)
{
    if(auto p = other.lock())
    {
        if(auto ptr = std::dynamic_pointer_cast<MouseCollider>(p))
        {
            on_pressed_func(other);
        }
    }
}

void Button::on_mouse_entered(const std::weak_ptr<Collider> other)
{
    if(auto p = other.lock())
    {
        if(auto t_area = p->get_parent().lock())
        {
            if(auto cursor = std::dynamic_pointer_cast<MouseCursor>(t_area->get_parent().lock()))
            {
                on_mouse_entered_func(other);
            }
        }
    }
}

void Button::on_mouse_left(const std::weak_ptr<Collider> other)
{
    if(auto p = other.lock())
    {
        if(auto t_area = p->get_parent().lock())
        {
            if(auto cursor = std::dynamic_pointer_cast<MouseCursor>(t_area->get_parent().lock()))
            {
                on_mouse_left_func(other);
            }
        }
    }
}

#pragma once
#include <SFML/Graphics.hpp>
#include "Icon.hpp"
#include "Label.hpp"
#include "Nodes/CollisionManager.hpp"


class Button : public Node, public sf::Transformable
{
private:
    using func = std::function<void(const std::weak_ptr<Collider>)>;

    void on_pressed(const std::weak_ptr<Collider> other);
    void on_mouse_entered(const std::weak_ptr<Collider> other);
    void on_mouse_left(const std::weak_ptr<Collider> other);

    func on_pressed_func;
    func on_mouse_entered_func;
    func on_mouse_left_func;

    std::string layer_name;
    std::shared_ptr<Collider> coll;
    std::shared_ptr<TriggerArea> trig;
    sf::Vector2f size;

public:
    std::shared_ptr<Icon> ico;
    std::shared_ptr<Label> label;
    Button(const std::string layer_name, const sf::Font& font, const sf::Texture& texture, sf::Vector2f size,
           func on_pressed, func on_mouse_entered = nullptr, func on_mouse_left = nullptr,
           const std::string& content = "", unsigned int font_size = 30, sf::IntRect rect = {{0, 0}, {-1, -1}});
    void initialize() override;
    void update(float delta) override;
};
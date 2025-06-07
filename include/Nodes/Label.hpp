#pragma once
#include "Nodes/Node.hpp"
#include <SFML/Graphics.hpp>

class Label : public Node
{
public:
    Label(const std::string layer_name, const sf::Font& font, unsigned int size = 30, const std::string& content = "");

    void set_position(const sf::Vector2f& position);
    void set_color(const sf::Color& color);
    void set_string(const std::string& str);
    void set_character_size(unsigned int size);

    sf::FloatRect get_global_bounds() const;
    std::string get_text();

    void update(float) override;

private:
    std::shared_ptr<sf::Text> text;
};
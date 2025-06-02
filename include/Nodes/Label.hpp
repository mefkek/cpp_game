#pragma once
#include <SFML/Graphics.hpp>

class Label {
public:
    Label(const std::string& content, const sf::Font& font, unsigned int size);

    void set_position(const sf::Vector2f& position);
    void set_color(const sf::Color& color);
    sf::Text& get_text();
    void render(sf::RenderTarget& target);

private:
    sf::Text text;
};
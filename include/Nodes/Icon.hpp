#pragma once
#include <SFML/Graphics.hpp>

class Icon {
public:
    Icon(const sf::Texture& texture);

    void set_position(const sf::Vector2f& position);
    void render(sf::RenderTarget& target);

private:
    sf::Sprite sprite;
};
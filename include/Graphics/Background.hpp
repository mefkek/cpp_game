#pragma once

#include <SFML/Graphics.hpp>
#include <string>

class Background {
public:
    Background();

    explicit Background(const std::string& filename);

    void setPosition(float x, float y);

    void draw(sf::RenderWindow& window) const;

private:
    sf::Texture texture_;
    sf::Sprite  sprite_;
};
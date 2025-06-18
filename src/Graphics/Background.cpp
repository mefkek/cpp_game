#include "Graphics/Background.hpp"
#include <stdexcept>

Background::Background()
    : texture_()
    , sprite_(texture_)  {}

Background::Background(const std::string& filename)
    : Background(){
    if (!texture_.loadFromFile(filename)) {
        throw std::runtime_error("Failed to load background " + filename);
    }
    sprite_.setTexture(texture_);
    }
void Background::setPosition(float x, float y) {
    sprite_.setPosition(sf::Vector2f(x, y));
}
void Background::draw(sf::RenderWindow& window) const {
    window.draw(sprite_);
}
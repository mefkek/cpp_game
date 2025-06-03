#include "Nodes/Icon.hpp"

Icon::Icon(const sf::Texture& texture) : sprite(texture) {
}
void Icon::set_position(const sf::Vector2f& position) {
    sprite.setPosition(position);
}
void Icon::render(sf::RenderTarget& target) {
    target.draw(sprite);
}
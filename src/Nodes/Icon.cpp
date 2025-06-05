#include "Nodes/Icon.hpp"

Icon::Icon(const sf::Texture& texture, RenderManager& manager, const std::string& layer)
    : sprite(texture), renderManager(manager), layerName(layer) {
    attach_to_layer();                                              // Add to the RenderManager layer on creation.
}

Icon::~Icon() {
    detach_from_layer();                                // Ensure icon is detached from the layer when destroyed
}

void Icon::set_position(const sf::Vector2f& position)
{
    sprite.setPosition(position);                                       // Set the icon's position
}

void Icon::attach_to_layer()
{
    renderManager.add_drawable(layerName, shared_from_this());      // Attach to layer
}

void Icon::detach_from_layer()
{
    renderManager.remove_drawable(layerName, shared_from_this()); // Detach from the layer
}

void Icon::update(float delta)
{
    sprite.move(10.0f * delta, 0.0f);                                 // Move icon to the right
}

void Icon::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(sprite, states);                                        // Draw the icons sprite on the screen
}
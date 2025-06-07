#include "Nodes/Icon.hpp"
#include "Nodes/RenderManager.hpp"
#include "Nodes/Application.hpp"

Icon::Icon(const std::string& layer) : layerName(layer) {}

Icon::Icon(const std::string& layer, const sf::Texture& texture, sf::IntRect rect)
    : layerName(layer)
{
    sprite = std::make_shared<sf::Sprite>(texture);
    if(rect.size.x != -1 && rect.size.y != -1)
    {
        sprite->setTextureRect(rect);
    }
    Application::instance().get_manager<RenderManager>()->add_drawable(layerName, sprite);
}

void Icon::set_position(const sf::Vector2f& position)
{
    sprite->setPosition(position);
}

void Icon::set_scale(const sf::Vector2f& scale)
{
    sprite->setScale(scale);
}

void Icon::set_texture(const sf::Texture& texture, sf::IntRect rect)
{
    sprite = std::make_shared<sf::Sprite>(texture);
    if(rect.size.x != -1 && rect.size.y != -1)
    {
        sprite->setTextureRect(rect);
    }
    Application::instance().get_manager<RenderManager>()->add_drawable(layerName, sprite);
}

sf::FloatRect Icon::get__global_bounds() const
{
    return sprite->getGlobalBounds();
}

sf::Vector2f Icon::get_position() const
{
    return sprite->getPosition();
}

void Icon::update(float delta){}
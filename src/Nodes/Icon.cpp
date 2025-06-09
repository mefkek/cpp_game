#include "Nodes/Icon.hpp"
#include "Nodes/RenderManager.hpp"
#include "Nodes/Application.hpp"

Icon::Icon(const std::string& layer, const sf::Texture& texture, sf::IntRect rect)
    : layer_name(layer), sf::Sprite(texture)
{
    if(rect.size.x != -1 && rect.size.y != -1)
    {
        setTextureRect(rect);
    }
}

void Icon::initialize()
{
    Application::instance().get_manager<RenderManager>()->add_drawable(layer_name, std::dynamic_pointer_cast<sf::Sprite>(shared_from_this()));
}

void Icon::update(float delta){}
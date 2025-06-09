#include "Nodes/Label.hpp"
#include "Nodes/RenderManager.hpp"
#include "Nodes/Application.hpp"

Label::Label(const std::string layer_name, const sf::Font& font, const std::string& content,  unsigned int size)
            : layer_name(layer_name), sf::Text(font, content, size){}

void Label::initialize()
{
    Application::instance().get_manager<RenderManager>()->add_drawable(layer_name, std::dynamic_pointer_cast<sf::Text>(shared_from_this()));
}

void Label::update(float) {}
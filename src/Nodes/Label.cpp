#include "Nodes/Label.hpp"
#include "Nodes/RenderManager.hpp"
#include "Nodes/Application.hpp"

Label::Label(const std::string layer_name, const sf::Font& font, unsigned int size, const std::string& content)
{
    text = std::make_shared<sf::Text>(font);
    text->setString(content);
    text->setCharacterSize(size);
    text->setFillColor(sf::Color::White);

    Application::instance().get_manager<RenderManager>()->add_drawable(layer_name, text);
}

void Label::set_position(const sf::Vector2f& position) {
    text->setPosition(position);
}

void Label::set_color(const sf::Color& color) {
    text->setFillColor(color);
}

void Label::set_string(const std::string& str)
{
    text->setString(str);
}

void Label::set_character_size(unsigned int size)
{
    text->setCharacterSize(size);
}

sf::FloatRect Label::get_global_bounds() const
{
    return text->getGlobalBounds();
}

std::string Label::get_text()
{
    return text->getString();
}

void Label::update(float) {}
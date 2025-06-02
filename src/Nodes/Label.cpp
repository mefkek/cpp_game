#include "Nodes/Label.hpp"

Label::Label(const std::string& content, const sf::Font& font, unsigned int size)
    : text(font, "", size)
{
    text.setFont(font);
    text.setString(content);
    text.setCharacterSize(size);
    text.setFillColor(sf::Color::White);
}

void Label::set_position(const sf::Vector2f& position) {
    text.setPosition(position);
}

void Label::set_color(const sf::Color& color) {
    text.setFillColor(color);
}

sf::Text& Label::get_text() {
    return text;
}

void Label::render(sf::RenderTarget& target) {
    target.draw(text);
}
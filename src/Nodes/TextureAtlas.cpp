#include "Nodes/TextureAtlas.hpp"

TextureAtlas::TextureAtlas(const std::filesystem::path& filepath) {
    text = std::make_unique<sf::Texture>();
    if (!text->loadFromFile(filepath)) {
        throw std::runtime_error("Failed to load texture atlas from: " + filepath.string());   // temporary without logger
    }
}
void TextureAtlas::set_rect(const std::unique_ptr<sf::Sprite>& sprite, sf::IntRect rect) {
    if (!sprite) {
        sprite -> setTexture(*text);
        sprite -> setTextureRect(rect);
    }
}
const std::unique_ptr<sf::Texture>& TextureAtlas::get_texture(){return text;}





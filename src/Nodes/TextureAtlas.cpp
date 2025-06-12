#include "Nodes/TextureAtlas.hpp"

TextureAtlas::TextureAtlas(const std::filesystem::path& filepath, sf::Vector2i tile_size) : tile_size(tile_size)
{
    tex = std::make_unique<sf::Texture>();
    if(!tex->loadFromFile(filepath))
    {
        throw std::runtime_error("Failed to load texture atlas from: " + filepath.string());
    }
}

void TextureAtlas::set_rect(std::shared_ptr<sf::Sprite>& sprite, sf::IntRect rect) const
{
    if(!sprite)
    {
        sprite = std::make_shared<sf::Sprite>(*tex);
    }
    else
    {
        sprite->setTexture(*tex);
    }
    sprite->setTexture(*tex);
    sprite->setTextureRect(rect);
}

void TextureAtlas::set_rect(std::shared_ptr<sf::Sprite>& sprite, sf::Vector2i cords) const
{
    if(!sprite)
    {
        sprite = std::make_shared<sf::Sprite>(*tex);
    }
    else
    {
        sprite->setTexture(*tex);
    }

    cords.x *= tile_size.x;
    cords.y *= tile_size.y;
    sf::IntRect tex_rect = {cords, tile_size};
    sprite->setTextureRect(tex_rect);
}

const sf::Texture& TextureAtlas::get_texture() const
{
    return *tex;
}





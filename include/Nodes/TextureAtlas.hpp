#pragma once
#include <SFML/Graphics.hpp>
#include <memory>

class TextureAtlas
{
    public:
    TextureAtlas(const std::filesystem::path&, sf::Vector2i tile_size = {16, 16});

    void set_rect(std::shared_ptr<sf::Sprite>& sprite, sf::IntRect rect) const;
    void set_rect(std::shared_ptr<sf::Sprite>& sprite, sf::Vector2i cords) const;

    const sf::Texture& get_texture() const;

    private:
    std::unique_ptr<sf::Texture> tex;
    sf::Vector2i tile_size;
};




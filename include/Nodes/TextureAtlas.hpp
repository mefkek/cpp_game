#pragma once
#include <SFML/Graphics.hpp>
#include <memory>

class TextureAtlas {
public:
    TextureAtlas(const std::filesystem::path&);

    // Here add a logger message if texture not found

    void set_rect(const std::unique_ptr<sf::Sprite>& sprite, sf::IntRect rect);

    const std::unique_ptr<sf::Texture>& get_texture();

private:
    std::unique_ptr<sf::Texture> text;
};




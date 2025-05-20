#pragma once
#include <SFML/Graphics.hpp>
#include <memory>

class TextureAtlas {
public:
    TextureAtlas(const std::filesystem::path&);
    void set_rect(const std::unique_ptr<sf::Sprite>&, sf::IntRect rect);
    const std::unique_ptr<sf::Texture>& get_texture();



private:
    std::unique_ptr<sf::Texture> text;



};




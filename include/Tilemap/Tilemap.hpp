#pragma once
#include <SFML/Graphics.hpp>
#include "Nodes/TextureAtlas.hpp"
#include <memory>
#include <array>

class Tilemap : public sf::Drawable, public sf::Transformable
{
    private:
    sf::VertexArray vertices;
    const sf::Texture* texture;

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    public:
    bool load(const TextureAtlas& atlas, sf::Vector2i tile_size, const sf::Vector2i* tiles, unsigned int width, unsigned int height);
};
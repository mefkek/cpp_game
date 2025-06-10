#include "Tilemap/Tilemap.hpp"
#include "Utility/Logger.hpp"

void Tilemap::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    states.texture = texture;
    target.draw(vertices, states);
}

bool Tilemap::load(const TextureAtlas& atlas, sf::Vector2i tile_size, const sf::Vector2i* tiles, unsigned int width, unsigned int height)
{
    texture = &atlas.get_texture();

    vertices.setPrimitiveType(sf::PrimitiveType::Triangles);
    vertices.resize(width * height * 6);

    for(unsigned int i = 0; i < width; ++i)
    {
        for(unsigned int j = 0; j < height; ++j)
        {
            const sf::Vector2i& tile_coords = tiles[i + j * width];

            sf::Vertex* triangles = &vertices[(i + j * width) * 6];

            triangles[0].position = sf::Vector2f(i * tile_size.x, j * tile_size.y);
            triangles[1].position = sf::Vector2f((i + 1) * tile_size.x, j * tile_size.y);
            triangles[2].position = sf::Vector2f(i * tile_size.x, (j + 1) * tile_size.y);
            triangles[3].position = sf::Vector2f(i * tile_size.x, (j + 1) * tile_size.y);
            triangles[4].position = sf::Vector2f((i + 1) * tile_size.x, j * tile_size.y);
            triangles[5].position = sf::Vector2f((i + 1) * tile_size.x, (j + 1) * tile_size.y);

            triangles[0].texCoords = sf::Vector2f(tile_coords.x * tile_size.x, tile_coords.y * tile_size.y);
            triangles[1].texCoords = sf::Vector2f((tile_coords.x + 1) * tile_size.x, tile_coords.y * tile_size.y);
            triangles[2].texCoords = sf::Vector2f(tile_coords.x * tile_size.x, (tile_coords.y + 1) * tile_size.y);
            triangles[3].texCoords = sf::Vector2f(tile_coords.x * tile_size.x, (tile_coords.y + 1) * tile_size.y);
            triangles[4].texCoords = sf::Vector2f((tile_coords.x + 1) * tile_size.x, tile_coords.y * tile_size.y);
            triangles[5].texCoords = sf::Vector2f((tile_coords.x + 1) * tile_size.x, (tile_coords.y + 1) * tile_size.y);
        }
    }

    return true;
}

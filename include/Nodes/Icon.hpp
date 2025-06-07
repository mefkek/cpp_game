#pragma once
#include <SFML/Graphics.hpp>
#include "Node.hpp"
#include <memory>

class Icon : public Node
{
private:
    std::shared_ptr<sf::Sprite> sprite;
    std::string layerName;
public:
    Icon(const std::string& layer);
    Icon(const std::string& layer, const sf::Texture& texture, sf::IntRect rect = {{0, 0}, {-1, -1}});

    void set_position(const sf::Vector2f& position);
    void set_scale(const sf::Vector2f& scale);
    void set_texture(const sf::Texture& texture, sf::IntRect rect = {{0, 0}, {-1, -1}});

    sf::FloatRect get__global_bounds() const;
    sf::Vector2f get_position() const;

    void update(float delta) override;
};
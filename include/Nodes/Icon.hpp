#pragma once
#include <SFML/Graphics.hpp>
#include "Node.hpp"
#include "Nodes/RenderManager.hpp"

class Icon : public Node, public sf::Drawable {
public:
    Icon(const sf::Texture& texture, RenderManager& manager, const std::string& layer);
    ~Icon();

    void set_position(const sf::Vector2f& position);
    void attach_to_layer();
    void detach_from_layer();

    void update(float delta) override;

private:
    sf::Sprite sprite;
    RenderManager& renderManager;
    std::string layerName;

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};
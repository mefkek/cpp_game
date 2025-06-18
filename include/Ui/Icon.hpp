#pragma once
#include <SFML/Graphics.hpp>
#include "../Nodes/Node.hpp"
#include <memory>

class Icon : public virtual Node, public sf::Sprite
{
    private:
    std::string layer_name;
    public:
    Icon(const std::string& layer, const sf::Texture& texture, sf::IntRect rect = {{0, 0}, {-1, -1}});
    void initialize() override;

    void update(float delta) override;
};
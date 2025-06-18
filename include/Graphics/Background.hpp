#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include "Nodes/Application.hpp"

class Background : public Node, public sf::Sprite
{
    public:
    using sf::Sprite::Sprite;

    void initialize() override;
    void update(float delta) override;
};
#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include "Node.hpp"

class FPSCounter : public Node
{
    private:
    sf::Font font;
    public:
    std::shared_ptr<sf::Text> text; //testing only
    FPSCounter();
    void set_position(sf::Vector2f pos);
    void update(float delta) override;
};
#pragma once

#include "Node.hpp"
#include <SFML/Graphics.hpp>
#include <memory>

class FPSCounter : public Node
{
    private:
    sf::Font font;
    float time_passed;
    unsigned int frames;

    public:
    std::shared_ptr<sf::Text> text; //testing only, add get/set functions after menagers are set up

    FPSCounter();

    void set_position(sf::Vector2f pos);
    
    void update(float delta) override;
};
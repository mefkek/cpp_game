#pragma once

#include "Node.hpp"
#include "Nodes/Label.hpp"
#include <SFML/Graphics.hpp>
#include <memory>

class FPSCounter : public Node
{
private:
    sf::Font font;
    float time_passed;
    unsigned int frames;

public:
    std::shared_ptr<Label> label;

    FPSCounter();
    void initialize();

    void set_position(sf::Vector2f pos);

    void update(float delta) override;
};
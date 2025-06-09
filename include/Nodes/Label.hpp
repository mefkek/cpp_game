#pragma once
#include "Nodes/Node.hpp"
#include <SFML/Graphics.hpp>

class Label : public virtual Node, public sf::Text
{
private:
    std::string layer_name;

public:
    Label(const std::string layer_name, const sf::Font& font, const std::string& content = "", unsigned int size = 30);
    void initialize() override;

    void update(float) override;
};
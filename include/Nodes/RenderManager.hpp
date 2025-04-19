#pragma once
#include <map>
#include <memory>
#include <SFML/Graphics.hpp>
#include "Node.hpp"

struct RenderLayer
{
    std::string name;   //acts as unique identifier, throw exception when duplicate
    std::vector<std::weak_ptr<sf::Drawable>> drawables;
    RenderLayer(std::string name);
};

class RenderManager : public Node
{
    private:
    std::map<char, std::vector<RenderLayer>> layers;
    std::map<char, sf::RenderTexture> renders;
    public:
    void add_layer(std::string name, char priority, sf::Vector2u size = {640, 360});
    void add_drawable(std::string layer, const std::weak_ptr<sf::Drawable>& dw);
    void remove_drawable(std::string layer, const std::weak_ptr<sf::Drawable>& dw);
    void remove_layer(std::string name);
    void update(float delta) override;
};
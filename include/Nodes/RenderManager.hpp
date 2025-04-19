#pragma once
#include <map>
#include <memory>
#include <SFML/Graphics.hpp>
#include "Node.hpp"

struct RenderLayer
{
    std::string name;
    sf::RenderTexture texture;
    std::vector<std::weak_ptr<sf::Drawable>> drawables;
    RenderLayer(std::string name);
};

class RenderManager : public Node
{
    private:
    std::map<char, std::vector<RenderLayer>> layers;
    public:
    void add_layer(std::string name, char priority);
    void add_drawable(std::string layer, std::weak_ptr<sf::Drawable> dw);
    void remove_drawable(std::string layer, std::weak_ptr<sf::Drawable> dw);
    void remove_layer(std::string name);
    void update(float delta) override;
};
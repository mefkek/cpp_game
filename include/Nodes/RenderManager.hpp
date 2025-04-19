#pragma once
#include <map>
#include <memory>
#include <SFML/Graphics.hpp>
#include "Node.hpp"

/*
    things to check:
        1. anything to do with removing layers or drawables
        2. what happends when you resize the window, add a layer, and then resize it again
        3. add comically large amounts of layers and drawables and see what happnes
        4. if view moves
*/

struct RenderLayer
{
    std::string name;   //acts as unique identifier, throw exception when duplicate
    std::vector<std::weak_ptr<sf::Drawable>> drawables;
    RenderLayer(std::string name);
};

struct Render
{
    std::unique_ptr<sf::Sprite> sp;
    std::unique_ptr<sf::RenderTexture> tex;
    Render(sf::RenderTexture n_tex);
    Render() = default;
};

class RenderManager : public Node
{
    private:
    std::map<char, std::vector<RenderLayer>> layers;
    std::map<char, Render> renders;
    sf::RenderWindow* window_ptr;
    public:
    void set_window(sf::RenderWindow* window);
    sf::Vector2u get_render_texture(std::string name);
    void add_layer(std::string name, char priority = 0, sf::Vector2u size = {640, 360});
    void add_drawable(std::string layer, const std::weak_ptr<sf::Drawable>& dw);
    void remove_drawable(std::string layer, const std::weak_ptr<sf::Drawable>& dw);
    void remove_layer(std::string name);
    void move_view(std::string layer_name, sf::Vector2f offset);
    void update(float delta) override;
};
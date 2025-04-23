#pragma once
#include <map>
#include <memory>
#include <SFML/Graphics.hpp>
#include "Node.hpp"

struct RenderLayer
{
    /*
        Class for staring information related to one layer
        Should maybe be moved into separate file
    */
    std::unique_ptr<sf::Sprite> sp;
    std::unique_ptr<sf::RenderTexture> tex;
    std::vector<std::weak_ptr<sf::Drawable>> drawables;
    RenderLayer(sf::Vector2u size);
    RenderLayer() = default;
};

class RenderManager : public Node
{
    private:
    std::map<unsigned char, RenderLayer> layers;
    std::map<std::string, unsigned char> string_ref;
    //Pointer for resizing, as resizing could happen in Application constructor and couse bugs
    sf::RenderWindow* window_ptr;

    public:
    void set_window(sf::RenderWindow* window);
    sf::Vector2u get_render_texture_size(const std::string& name);
    void add_layer(const std::string& name, char priority = 0, sf::Vector2u size = {640, 360});
    void add_drawable(const std::string& layer, const std::weak_ptr<sf::Drawable>& dw);
    void remove_drawable(const std::string& layer, const std::weak_ptr<sf::Drawable>& dw);
    void remove_layer(const std::string& name);
    void move_view(const std::string& layer, sf::Vector2f offset);
    void rescale();
    void update(float delta) override;
};
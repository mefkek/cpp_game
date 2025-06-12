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

    public:
    sf::RenderTexture& get_render_texture(const std::string& name);
    sf::Sprite& get_render_sprite(const std::string& name);

    sf::Vector2f translate_to_layer(const std::string& layer, const sf::Vector2f& vec);

    void add_layer(const std::string& name, unsigned char priority = 0, sf::Vector2u size = {640, 360});

    void add_drawable(const std::string& layer, const std::weak_ptr<sf::Drawable>& dw);

    void remove_drawable(const std::string& layer, const std::weak_ptr<sf::Drawable>& dw);

    void remove_layer(const std::string& name);

    void move_view(const std::string& layer, sf::Vector2f offset);  //basically a camera

    void rescale();

    void update(float delta) override;
};
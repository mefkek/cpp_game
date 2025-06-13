#pragma once
#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>
#include "TextureAtlas.hpp"
#include "Node.hpp"

class Animation : public Node
{
private:
    std::shared_ptr<sf::Sprite> sprite;
    const TextureAtlas& atlas;

    std::vector<sf::Vector2i> frames;
    float frame_time;
    float elapsed_time = 0.f;
    size_t current_frame = 0;
    bool loop = true;

public:
    Animation(std::shared_ptr<sf::Sprite> sprite,
                       const TextureAtlas& atlas,
                       const std::vector<sf::Vector2i>& frames,
                       float frame_time,
                       bool loop = true);

    void update(float delta) override;
    void reset();
};
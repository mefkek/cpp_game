#include "Graphics/AnimationHelper.hpp"
#include "Graphics/Animation.hpp"

std::shared_ptr<Animation> create_example_animation(const TextureAtlas& atlas) {
    std::vector<sf::IntRect> frames;
    constexpr int NUM_FRAMES = 4;

    for (int i = 0; i < NUM_FRAMES; ++i) {
        sf::IntRect frame(
            sf::Vector2i(static_cast<int>(i * FRAME_WIDTH), 0),
            sf::Vector2i(static_cast<int>(FRAME_WIDTH), static_cast<int>(FRAME_HEIGHT))
        );
        frames.emplace_back(frame);
    }

    auto sprite = std::make_shared<sf::Sprite>(atlas.get_texture());

    if (!frames.empty()) {
        atlas.set_rect(sprite, frames[0]);
    }

    return std::make_shared<Animation>(sprite, atlas, frames, 0.1f, true);
}
#include "Graphics/Animation.hpp"
#include <SFML/Graphics.hpp>
#include "Nodes/TextureAtlas.hpp"

Animation::Animation(std::shared_ptr<sf::Sprite> animated_sprite,
                     const TextureAtlas& texture_atlas,
                     const std::vector<sf::IntRect>& animation_frames,
                     float frame_duration,
                     bool is_looping)
    : sprite(animated_sprite), atlas(texture_atlas), frames(animation_frames),
      frame_time(frame_duration), loop(is_looping), elapsed_time(0.f), current_frame(0) {
    set_first_frame();
}

void Animation::update(float delta_time) {
    const size_t total_frames = frames.size();
    if (total_frames <= 1) return;

    elapsed_time += delta_time;


    if (elapsed_time >= frame_time) {
        elapsed_time -= frame_time;
        current_frame++;


        if (current_frame >= total_frames) {
            current_frame = loop ? 0 : total_frames - 1;
        }


        atlas.set_rect(sprite, frames[current_frame]);
    }
}

void Animation::reset() {
    elapsed_time = 0.f;
    current_frame = 0;
    set_first_frame();
}

void Animation::set_first_frame() {
    if (!frames.empty()) {
        atlas.set_rect(sprite, frames[0]);
    }
}

Animation Animation::load_animation(const std::string& texture_file,
                                    const std::vector<sf::IntRect>& frame_rects,
                                    float frame_duration,
                                    bool is_looping) {

    auto texture = std::make_unique<sf::Texture>();
    if (!texture->loadFromFile(texture_file)) {
        throw std::runtime_error("Texture load failed " + texture_file);
    }

    auto sprite = std::make_shared<sf::Sprite>(*texture);

    TextureAtlas texture_atlas(texture_file);

    return Animation(sprite, texture_atlas, frame_rects, frame_duration, is_looping);
}
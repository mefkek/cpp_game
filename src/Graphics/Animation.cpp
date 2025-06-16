#include <SFML/Graphics.hpp>
#include "Graphics/Animation.hpp"
#include "Nodes/TextureAtlas.hpp"
                                                                // Constructor to initialize Animation with given parameters
Animation::Animation(std::shared_ptr<sf::Sprite> animated_sprite,
                     const TextureAtlas &texture_atlas,
                     const std::vector<sf::IntRect> &animation_frames,
                     float frame_duration,
                     bool is_looping)
    : sprite(animated_sprite), atlas(texture_atlas), frames(animation_frames),
      frame_time(frame_duration), loop(is_looping), elapsed_time(0.f), current_frame(0) {
    set_first_frame();                                          // Set the first frame of the animation upon instantiation
}
                                                                // Update the animation according to the elapsed time
void Animation::update(float delta_time) {
    const size_t total_frames = frames.size();
    if (total_frames <= 1) return;                              // No animation needed if there are less than 2 frames

    elapsed_time += delta_time;
                                                                // Check if it is time to move to the next frame
    if (elapsed_time >= frame_time) {
        elapsed_time -= frame_time;
        current_frame++;
                                                                // Handle looping or stopping at the last frame
        if (current_frame >= total_frames) {
            current_frame = loop ? 0 : total_frames - 1;        // Reset to the first frame if looping
        }
                                                                // Update the sprite's texture rectangle to match the current frame
        atlas.set_rect(sprite, frames[current_frame]);
    }
}
                                                                // Reset the animation to its initial state
void Animation::reset() {
    elapsed_time = 0.f;
    current_frame = 0;
    set_first_frame();                                          // Reset the sprite to the first frame
}
                                                                // Privately sets the sprite to the first frame in the animation
void Animation::set_first_frame() {
    if (!frames.empty()) {
        atlas.set_rect(sprite, frames[0]);                   // Set the rectangle to the first frame
    }
}
                                                                // Static method to load an animation from a texture file and frame definitions
Animation Animation::load_animation(const std::string &texture_file,
                                    const std::vector<sf::IntRect> &frame_rects,
                                    float frame_duration,
                                    bool is_looping) {
                                                                // Create and load texture from the given file
    auto texture = std::make_unique<sf::Texture>();
    if (!texture->loadFromFile(texture_file)) {
        throw std::runtime_error("Failed to load texture file: " + texture_file);
    }
                                                                // Create a shared sprite using the loaded texture
    auto sprite = std::make_shared<sf::Sprite>(*texture);

                                        // Create a texture atlas using the texture's file path (TextureAtlas handles file logic)
    TextureAtlas texture_atlas(texture_file);
                                                                // Return a new Animation object
    return Animation(sprite, texture_atlas, frame_rects, frame_duration, is_looping);
}
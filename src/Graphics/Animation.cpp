#include "Animation§.hpp"

AnimationComponent::AnimationComponent(std::shared_ptr<sf::Sprite> sprite,
                                       const TextureAtlas& atlas,
                                       const std::vector<sf::Vector2i>& frames,
                                       float frame_time,
                                       bool loop)
    : sprite(sprite), atlas(atlas), frames(frames), frame_time(frame_time), loop(loop)
{
    if (!frames.empty()) {
        atlas.set_rect(sprite, frames[0]);
    }
}

void AnimationComponent::update(float delta)
{
    if (frames.size() <= 1) return;

    elapsed_time += delta;
    if (elapsed_time >= frame_time)
    {
        elapsed_time -= frame_time;
        current_frame++;

        if (current_frame >= frames.size()) {
            if (loop)
                current_frame = 0;
            else
                current_frame = frames.size() - 1;
        }

        atlas.set_rect(sprite, frames[current_frame]);
    }
}

void AnimationComponent::reset()
{
    elapsed_time = 0.f;
    current_frame = 0;
    if (!frames.empty()) {
        atlas.set_rect(sprite, frames[0]);
    }
}
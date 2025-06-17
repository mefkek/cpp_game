#pragma once

#include <SFML/Graphics.hpp>
#include "Nodes/TextureAtlas.hpp"
#include "Graphics/Animation.hpp"
#include <vector>
#include <memory>

class Animation;

constexpr int FRAME_WIDTH = 16;
constexpr int FRAME_HEIGHT = 16;

std::shared_ptr<Animation> create_example_animation(const TextureAtlas& atlas);
#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <random>
#include <cmath>

constexpr int dungeon_min = 6u;

struct DungeonRect
{
    sf::IntRect rect;
    std::shared_ptr<DungeonRect> left;
    std::shared_ptr<DungeonRect> right;

    void divide(std::mt19937& rd_dev, sf::Vector2i division_axis, unsigned int chunk_size);

    void divide_along(int split_pos, bool horizontal, unsigned int chunk_size);
};
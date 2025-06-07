#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <random>
#include <cmath>

constexpr int get_min_separation(int chunk_size);

struct DungeonRect
{
    sf::IntRect rect;
    std::shared_ptr<DungeonRect> left;
    std::shared_ptr<DungeonRect> right;

    void divide(std::mt19937& rd_dev, sf::Vector2i division_axis, int chunk_size);

    void divide_along(int split_pos, bool horizontal, int chunk_size);
};

#include "DungeonRect.inl"
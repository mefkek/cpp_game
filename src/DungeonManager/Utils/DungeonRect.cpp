#include "DungeonManager/Utils/DungeonRect.hpp"

void DungeonRect::divide(std::mt19937 &rd_dev, sf::Vector2i division_axis, unsigned int chunk_size)
{
    sf::Vector2i bounds = rect.size;
    sf::Vector2i offset = rect.position;

    unsigned int d_bound = (division_axis.x != 0) ? rect.size.x : rect.size.y;
    long long min_div_size = std::min(dungeon_min, 2 * (static_cast<int>(chunk_size) / 5));
    if (min_div_size % 2 != 0)
    {
        ++min_div_size;
    }

    std::uniform_int_distribution dist(min_div_size, d_bound - min_div_size);
    long long d_pos = dist(rd_dev);

    sf::IntRect l_rect;
    l_rect.position = offset;
    l_rect.size = (division_axis.x != 0) ? sf::Vector2i(d_pos, bounds.y) : sf::Vector2i(bounds.x, d_pos);

    sf::IntRect r_rect;
    r_rect.position = {offset.x + (l_rect.size.x * division_axis.x), offset.y + (l_rect.size.y * division_axis.y)};
    r_rect.size = {bounds.x - (l_rect.size.x * division_axis.x), bounds.y - (l_rect.size.y * division_axis.y)};

    if (l_rect.size.x >= min_div_size && l_rect.size.y >= min_div_size && r_rect.size.x >= min_div_size && r_rect.size.y >= min_div_size)
    {
        left = std::make_shared<DungeonRect>();
        left->rect = l_rect;

        right = std::make_shared<DungeonRect>();
        right->rect = r_rect;
    }
}

void DungeonRect::divide_along(int split_pos, bool horizontal, unsigned int chunk_size)
{
    sf::Vector2i bounds = rect.size;
    sf::Vector2i offset = rect.position;

    long long min_div_size = std::min(dungeon_min, 2 * (static_cast<int>(chunk_size) / 5));
    if (min_div_size % 2 != 0)
    {
        ++min_div_size;
    }

    if (horizontal)
    {
        // Global to local conversion
        int local_split = split_pos - offset.y;

        if (local_split <= min_div_size || bounds.y - local_split <= min_div_size)
        {
            return;
        }

        sf::IntRect l_rect{offset, sf::Vector2i(bounds.x, local_split)};
        sf::IntRect r_rect{sf::Vector2i(offset.x, offset.y + local_split),
                           sf::Vector2i(bounds.x, bounds.y - local_split)};

        left = std::make_shared<DungeonRect>();
        left->rect = l_rect;

        right = std::make_shared<DungeonRect>();
        right->rect = r_rect;
    }
    else
    {
        int local_split = split_pos - offset.x;

        if (local_split <= min_div_size || bounds.x - local_split <= min_div_size)
        {
            return;
        }

        sf::IntRect l_rect{offset, sf::Vector2i(local_split, bounds.y)};
        sf::IntRect r_rect{sf::Vector2i(offset.x + local_split, offset.y),
                           sf::Vector2i(bounds.x - local_split, bounds.y)};

        left = std::make_shared<DungeonRect>();
        left->rect = l_rect;

        right = std::make_shared<DungeonRect>();
        right->rect = r_rect;
    }
}
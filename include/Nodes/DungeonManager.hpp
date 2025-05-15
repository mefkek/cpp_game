#pragma once
#include <SFML/Graphics.hpp>
#include <cstdint>
#include <vector>
#include <array>
#include "Node.hpp"

class DungeonManager : public Node
{
    private:
    enum RoomType
    {
        Empty = 0,
        Treasure,
        Heal,
        Trap,
        Trader
    };

    class Room
    {
        public:
        RoomType type;
        std::array<std::pair<sf::Vector2i, std::uint32_t>, 4> exits;
    };

    class Corridor : public Room
    {
        public:
        bool vertical;
        bool intersection;
    };

    class Chunk
    {
        public:
        std::array<std::pair<sf::Vector2i, std::uint32_t>, 4> exits;
        std::vector<std::shared_ptr<Room>> rooms;
    };

    std::uint32_t chunk_size;
    sf::Vector2<std::uint32_t> dungeon_size;

    std::size_t dungeon_seed;

    public:
    void initialize() override;
    void update(float delta) override {}
    Chunk get_chunk(sf::Vector2<std::int64_t> position);
};
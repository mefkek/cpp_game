#pragma once
#include <SFML/Graphics.hpp>
#include <cstdint>
#include <vector>
#include <array>
#include "Node.hpp"

class DungeonManager : public Node
{
    private:
    std::uint32_t chunk_size;
    sf::Vector2<std::uint32_t> dungeon_size;

    std::size_t dungeon_seed;

    public:

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
        sf::Vector2<std::uint32_t> position;
        std::vector<sf::Vector2i> exits;
        RoomType type;
        Room(sf::Vector2<std::uint32_t> pos = {0, 0}, std::vector<sf::Vector2i> exits = {});
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
        std::array<std::shared_ptr<Room>, 4> exits;
        std::vector<std::shared_ptr<Room>> rooms;
    };
    
    void initialize() override;
    void update(float delta) override {}
    Chunk get_chunk(sf::Vector2<std::int64_t> position);
};
#pragma once
#include <SFML/Graphics.hpp>
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
        std::vector<sf::Vector2i> exits;
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
        std::array<int, 4> exits;
        std::vector<std::shared_ptr<Room>> rooms;
    };

    sf::Vector2u chunk_size;
    sf::Vector2u dungeon_size;

    unsigned int dungeon_seed;

    public:
    DungeonManager();
    void initialize() override;
    void update(float delta) override {}
    Chunk get_chunk(sf::Vector2i position);
};
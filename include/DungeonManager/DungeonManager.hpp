#pragma once
#include <SFML/Graphics.hpp>
#include <cstdint>
#include <vector>
#include <array>
#include "Nodes/Node.hpp"
#include "RoomVisualizer.hpp"

#include "ChunkGenerator.hpp"

class DungeonManager : public Node
{
    private:
    unsigned int chunk_size;
    sf::Vector2u dungeon_size;
    std::size_t dungeon_seed;
    sf::Vector2i pos_dungeon;
    sf::Vector2i pos_chunk;
    std::shared_ptr<Chunk> chunk;
    std::unique_ptr<RoomVisualizer> visualizer;

    public:
    std::unique_ptr<ChunkGenerator> chunk_getter;
    void initialize() override;
    void update(float delta) override{}
    bool move(sf::Vector2i diff);
};
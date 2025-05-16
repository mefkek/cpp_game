#pragma once
#include <SFML/Graphics.hpp>
#include <cstdint>
#include <vector>
#include <array>
#include "Nodes/Node.hpp"

#include "ChunkGenerator.hpp"

class DungeonManager : public Node
{
    private:
    unsigned int chunk_size;
    sf::Vector2u dungeon_size;
    std::size_t dungeon_seed;

    public:
    std::unique_ptr<ChunkGenerator> chunk_getter;
    void initialize() override;
    void update(float delta) override{}
    void display_chunk(std::shared_ptr<Chunk>); //mostly likely for debugging only;
};
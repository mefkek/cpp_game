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
    using chunk_p = std::pair<sf::Vector2i, std::shared_ptr<Chunk>>;
    unsigned int chunk_size;
    sf::Vector2u dungeon_size;
    std::size_t dungeon_seed;
    sf::Vector2i pos_dungeon;
    sf::Vector2i pos_chunk;
    std::vector<chunk_p> loaded_chunks;
    std::unique_ptr<RoomVisualizer> visualizer;
    std::unique_ptr<ChunkGenerator> chunk_getter;
    std::shared_ptr<Chunk> current_chunk;
    void reload_chunks();

    public:
    void initialize() override;
    void update(float delta) override{}
    bool move(sf::Vector2i diff);
};
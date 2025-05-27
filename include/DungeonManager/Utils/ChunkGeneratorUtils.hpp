#pragma once
#include "../ChunkGenerator.hpp"
#include <SFML/Graphics.hpp>
#include <random>

int sign(int v);

std::string get_edge_hash(sf::Vector2i a, sf::Vector2<std::int64_t> b, std::int64_t dungeon_seed,
                          sf::Vector2u dungeon_size);

void generate_exits(sf::Vector2i position, sf::Vector2u dungeon_size, std::size_t dungeon_seed, int chunk_size, 
                    std::vector<std::tuple<sf::Vector2i, sf::Vector2i, int>>& exit_closest,
                    std::shared_ptr<Chunk> n_chunk);

void generate_internal(std::shared_ptr<Chunk> n_chunk, std::mt19937& rd_dev, int chunk_size);
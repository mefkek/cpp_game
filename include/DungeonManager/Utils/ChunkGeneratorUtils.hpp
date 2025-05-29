#pragma once
#include "../ChunkGenerator.hpp"
#include <SFML/Graphics.hpp>
#include <random>

int sign(int v);

std::string get_edge_hash(sf::Vector2i a, sf::Vector2<std::int64_t> b, std::int64_t dungeon_seed,
                          sf::Vector2u dungeon_size);

void generate_exits(sf::Vector2i position, sf::Vector2u dungeon_size, std::size_t dungeon_seed, int chunk_size, 
                    std::vector<sf::Vector2i>& chunk_exits,
                    std::shared_ptr<Chunk> n_chunk);

void generate_internal(std::shared_ptr<Chunk> n_chunk, std::mt19937& rd_dev, int chunk_size);

sf::Vector2i find_closest(sf::Vector2i exit_pos, std::shared_ptr<Chunk> n_chunk, int chunk_size, const std::vector<sf::Vector2i>& o_exits, bool fallback = false);

void connect_exit(sf::Vector2i exit_pos, sf::Vector2i goal, std::shared_ptr<Chunk> n_chunk, int chunk_size);
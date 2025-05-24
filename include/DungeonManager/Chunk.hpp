#pragma once
#include "Room.hpp"
#include <map>

class Chunk
{
    public:
    //std::array<std::shared_ptr<Room>, 4> exits;
    std::map<int, std::map<int, std::shared_ptr<Room>>> rooms;
};
#pragma once
#include "Room.hpp"

class Chunk
{
    public:
    //std::array<std::shared_ptr<Room>, 4> exits;
    std::vector<std::shared_ptr<Room>> rooms;
};
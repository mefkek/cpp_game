constexpr int get_min_separation(int chunk_size)
{
    return std::min(6, chunk_size / 7);
}
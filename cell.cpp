#include "cell.hpp"
#include <functional>

std::size_t CellHash::operator()(const Cell& cell) const
{
    std::size_t h1 = std::hash<std::int16_t>{}(cell.x);
    std::size_t h2 = std::hash<std::int16_t>{}(cell.y);
    return h1 ^ (h2 << 1) ^ (h1 >> 1);
}

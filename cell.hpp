#pragma once
#include <cstddef>
#include <functional>

struct Cell {
    std::int16_t x, y;
    bool operator==(const Cell& other) const {
        return x == other.x && y == other.y;
    }
};

struct CellHash {
    std::size_t operator()(const Cell& seg) const {
        std::size_t h1 = std::hash<std::int16_t>{}(seg.x);
        std::size_t h2 = std::hash<std::int16_t>{}(seg.y);
        return h1 ^ (h2 << 1) ^ (h1 >> 1);
    }
};
#pragma once
#include <cstddef>
#include <functional>

struct Cell {
    short int x, y;
    bool operator==(const Cell& other) const {
        return x == other.x && y == other.y;
    }
};

struct CellHash {
    std::size_t operator()(const Cell& seg) const {
        std::size_t h1 = std::hash<short int>{}(seg.x);
        std::size_t h2 = std::hash<short int>{}(seg.y);
        return h1 ^ (h2 << 1) ^ (h1 >> 1);
    }
};
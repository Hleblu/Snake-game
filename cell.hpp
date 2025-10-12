#pragma once
#include <cstddef>
#include <functional>

struct Cell {
    std::int16_t x, y;
    constexpr bool operator==(const Cell& other) const {
        return x == other.x && y == other.y;
    }

    constexpr Cell operator+(const Cell& other) const {
        return Cell({
            static_cast<std::int16_t>(x + other.x),
            static_cast<std::int16_t>(y + other.y)
        });
    }

    constexpr Cell operator-(const Cell& other) const {
        return Cell({
            static_cast<std::int16_t>(x - other.x),
            static_cast<std::int16_t>(y - other.y)
        });
    }

    constexpr Cell(int x, int y) : x(static_cast<std::int16_t>(x)), y(static_cast<std::int16_t>(y)) {}

    constexpr Cell() : x(0), y(0) {}
};

struct CellHash {
    std::size_t operator()(const Cell& cell) const {
        std::size_t h1 = std::hash<std::int16_t>{}(cell.x);
        std::size_t h2 = std::hash<std::int16_t>{}(cell.y);
        return h1 ^ (h2 << 1) ^ (h1 >> 1);
    }
};
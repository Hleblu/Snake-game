#pragma once
#include <cstdint>
#include <cstddef>

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
    std::size_t operator()(const Cell& cell) const;
};
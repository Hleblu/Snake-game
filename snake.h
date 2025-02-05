#pragma once
#include "constants.h"
#include <vector>
#include <unordered_set>

class Snake : public sf::Drawable
{
    struct Segment {
        float x, y;
        bool operator==(const Segment& other) const {
            return x == other.x && y == other.y;
        }
    };

    struct SegmentHash {
        std::size_t operator()(const Segment& seg) const {
            std::size_t h1 = std::hash<float>{}(seg.x);
            std::size_t h2 = std::hash<float>{}(seg.y);
            return h1 ^ (h2 << 1);
        }
    };

    sf::Color snakeColor = { 41, 74, 242 };
    sf::VertexArray snakeVertices;
    float colorDecrementStep;
    std::vector<Segment> segments;
    std::unordered_set<Segment, SegmentHash> segmentsSet;
    enum Direction {
        NONE = -1,
        LEFT = 1,
        UP,
        RIGHT,
        DOWN
    } direction, previousDirection, nextDirection;

    Snake();
    void setDefaults();
    bool checkCollision();
    void grow();
    void move();
    void updateVertexArray();
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    friend class Game;
    friend class Apple;
};

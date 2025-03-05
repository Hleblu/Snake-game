#pragma once
#include "constants.h"
#include <deque>
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

    sf::Color segmentColor = { 41, 74, 242 };
    sf::VertexArray segmentsVertices;
    float colorDecrementStep;
    std::deque<Segment> segments;
    std::deque<Segment> previousSegments;
    std::unordered_multiset<Segment, SegmentHash> segmentsSet;
    bool firstMove;
    enum Direction {
        NONE = -1,
        LEFT = 1,
        UP,
        RIGHT,
        DOWN
    } direction, previousDirection, nextDirection;

    Snake();
    void restoreDefaultValues();
    bool hasCollided();
    void grow();
    void move();
    bool canUpdateDirection();
    void updateColors();
    void updateVertexArray(float dt = 0);
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    friend class Game;
    friend class Apple;
};

#pragma once
#include <SFML/Graphics.hpp>
#include "configuration.h"
#include <deque>
#include <unordered_set>

class Snake : public sf::Drawable
{
    struct Segment {
        short int x, y;
        bool operator==(const Segment& other) const {
            return x == other.x && y == other.y;
        }
    };

    struct SegmentHash {
        std::size_t operator()(const Segment& seg) const {
            std::size_t h1 = std::hash<short int>{}(seg.x);
            std::size_t h2 = std::hash<short int>{}(seg.y);
            return h1 ^ (h2 << 1);
        }
    };

    Configuration* config = Configuration::getInstance();
    sf::VertexArray segmentsVertices;
    std::deque<Segment> segments;
    std::deque<Segment> previousSegments;
    std::unordered_multiset<Segment, SegmentHash> segmentsSet;
    short int hashDelay;

    void updateTexCoords();

public:
    enum Direction {
        NONE = -1,
        LEFT = 1,
        UP,
        RIGHT,
        DOWN
    } direction, previousDirection, nextDirection;
    bool firstMove;

    Snake();
    void restoreDefaultValues();
    bool hasCollided() const;
    void grow(int size = 1);
    void move();
    bool canUpdateDirection() const;
    const std::deque<Segment>& getSegments() const;
    const std::deque<Segment>& getPrevSegments() const;
    const std::unordered_multiset<Segment, SegmentHash>& getSegmentsHash() const;
    void updateVertices(float dt = 0);
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

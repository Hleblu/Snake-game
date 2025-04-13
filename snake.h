#pragma once
#include <SFML/Graphics.hpp>
#include "configuration.h"
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

    sf::VertexArray segmentsVertices;
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
    Configuration* config = Configuration::getInstance();

    Snake();
    void restoreDefaultValues();
    bool hasCollided();
    void grow();
    void move();
    bool canUpdateDirection();
    void updateVertices(float dt = 0);
	void updateTexCoords();
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    friend class Game;
    friend class Apple;
};

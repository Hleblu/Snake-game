#pragma once
#include <SFML/Graphics.hpp>
#include <deque>
#include "cell.h"

class Snake : public sf::Drawable
{
    sf::VertexArray vertices;
    std::deque<Cell> segments;
    std::deque<Cell> previousSegments;
    std::uint16_t hashDelay;

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
    void grow(const int size = 1);
    void move();
    bool canUpdateDirection() const;
    const std::deque<Cell>& getSegments() const;
    void updateVertices(const float dt = 0);
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

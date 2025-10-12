#pragma once
#include "cell.hpp"
#include <deque>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/VertexArray.hpp>

class Snake : public sf::Drawable
{
    sf::VertexArray vertices;
    std::deque<Cell> segments;
    std::deque<Cell> prevSegments;
    std::uint16_t hashDelay;

    void updateTexCoords();

public:
    bool firstMove;
    enum class Direction {
        NONE = -1,
        LEFT = 1,
        UP,
        RIGHT,
        DOWN
    } direction, prevDirection, nextDirection;

    Snake();
    void restoreDefaultValues();
    bool hasCollided() const;
    void grow(const int size = 1);
    void move();
    bool canUpdateDirection() const;
    const std::deque<Cell>& getSegments() const;
    const size_t getSize() const;
    void updateVertices(const float dt = 0);
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

#pragma once
#include "cell.hpp"
#include <deque>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/VertexArray.hpp>

class Snake : public sf::Drawable
{
public:
    enum class Direction {
        NONE = -1,
        LEFT = 1,
        UP,
        RIGHT,
        DOWN
    };
private:
    sf::VertexArray vertices;
    sf::VertexArray jointVertices;
    std::deque<Cell> segments;
    std::deque<Cell> prevSegments;
    std::uint16_t tailCollisionDelay;
    bool firstMove;
    Direction direction, prevDirection, nextDirection;

    void updateTexCoords();
    void updateJointVertices();
    bool canUpdateDirection() const;
public:
    Snake();
    sf::Vector2f getPositionInfront() const;
    void restoreDefaultValues();
    bool hasCollided() const;
    bool firstTimeMoving() const;
    void grow(const int size = 1);
    void move();
    void setNextDirection(Direction dir);
    bool updateDirection();
    const size_t getSize() const;
    void updateVertices(const float dt = 0);
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
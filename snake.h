#pragma once
#include <SFML/Graphics.hpp>
#include "configuration.h"
#include <deque>
#include <unordered_set>
#include "cell.h"
#include "collisionManager.h"

class Snake : public sf::Drawable
{
    Configuration* config = Configuration::getInstance();
    CollisionManager* collisionManager = CollisionManager::getInstance();
    sf::VertexArray segmentsVertices;
    std::deque<Cell> segments;
    std::deque<Cell> previousSegments;
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
    void grow(const int size = 1);
    void move();
    bool canUpdateDirection() const;
    const std::deque<Cell>& getSegments() const;
    void updateVertices(const float dt = 0);
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

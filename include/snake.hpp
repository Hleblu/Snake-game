#pragma once
#include "cell.hpp"
#include "direction.hpp"
#include <deque>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/VertexArray.hpp>

class Configuration;

class CollisionManager;

class Snake : public sf::Drawable
{
private:
    Configuration* config;
    CollisionManager* collision;
    sf::Shader* shader;
    float shaderTime = 0.f;
    sf::VertexArray vertices;
    sf::VertexArray jointVertices;
    std::deque<Cell> segments;
    std::deque<Cell> lastSegments;
    std::uint16_t collisionPending;
    bool hasNotMovedYet;
    Direction dir, prevDir, nextDir;

    void updateTexCoords();
    void updateJointVertices();
    bool canUpdateDirection() const;
public:
    Snake(Configuration* config, CollisionManager* collision, sf::Shader* shader);
    sf::Vector2f getHeadCenter() const;
    Direction getDirection() const;
    void restoreDefaultValues();
    bool hasCollided() const;
    void triggerDeath(float currentTime);
    bool isWaitingForFirstMove() const;
    void grow(int size = 1);
    void move();
    void setNextDirection(Direction::Val dir);
    bool updateDirection();
    size_t getSize() const;
    Cell getHead() const;
    void updateShader(float currentTime);
    void updateVertices(float dt = 0);
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
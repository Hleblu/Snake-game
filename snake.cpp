#include "collisionManager.hpp"
#include "configuration.hpp"
#include "snake.hpp"
#include <SFML/Graphics/RenderTarget.hpp>
#include "randomGenerator.hpp"

constexpr std::size_t TRIANGLE_VERTICES = 6;

Snake::Snake()
{
    vertices.setPrimitiveType(sf::PrimitiveType::Triangles);
    restoreDefaultValues();
}

void Snake::restoreDefaultValues()
{
    const std::int16_t centerX = config.rows / 2;
    const std::int16_t centerY = config.columns / 2;

    segments = {
        {centerX, centerY},
        {centerX - 1, centerY},
        {centerX - 2, centerY}
    };
    prevSegments = segments;

    collisionManager.setOccupied(segments[0], ObjectType::SNAKE_HEAD);
    collisionManager.setOccupied(segments[1], ObjectType::SNAKE_TAIL);
    collisionManager.setOccupied(segments[2], ObjectType::SNAKE_TAIL);

    direction = Direction::NONE, prevDirection = Direction::NONE, nextDirection = Direction::NONE;
    firstMove = true;
    hashDelay = 0;

    vertices.clear();
    vertices.resize(config.rows * config.columns * TRIANGLE_VERTICES);

    updateVertices();
	updateTexCoords();
}

bool Snake::hasCollided() const
{
    const Cell& head = segments.front();
    return collisionManager.checkCellType(head, ObjectType::SNAKE_TAIL)
        || collisionManager.checkCellType(head, ObjectType::OBSTACLE)
        || collisionManager.isOutOfBorders(head);
}

void Snake::grow(const int size)
{
    hashDelay += size;
    const size_t newSize = segments.size() + size;
    segments.resize(newSize, segments.back());
    prevSegments.resize(newSize, segments.back());
	updateTexCoords();
}

void Snake::move()
{
    if (direction == Direction::NONE) return;
    
    if (!firstMove) { 
        prevSegments.pop_back();
        prevSegments.emplace_front(segments.front());
    }
    else firstMove = false;

    if (hashDelay == 0)
        collisionManager.setFree(segments.back(), ObjectType::SNAKE_TAIL);
    else hashDelay -= 1;
    
    segments.pop_back();
    segments.emplace_front(segments.front());
    collisionManager.changeTypes(segments.front(), ObjectType::SNAKE_HEAD, ObjectType::SNAKE_TAIL);

    switch (direction) {
        case Direction::RIGHT:
            segments[0].x++;
            break;
        case Direction::LEFT:
            segments[0].x--;
            break;
        case Direction::UP:
            segments[0].y--;
            break;
        case Direction::DOWN:
            segments[0].y++;
            break;
        default:
            break;
    }

    collisionManager.setOccupied(segments.front(), ObjectType::SNAKE_HEAD);
    prevDirection = direction;
}

bool Snake::canUpdateDirection() const
{
    return nextDirection != direction 
        && static_cast<int>(nextDirection) % 2 != static_cast<int>(prevDirection) % 2;
}

const std::deque<Cell>& Snake::getSegments() const 
{
    return segments;
}

const size_t Snake::getSize() const
{
    return segments.size();
}

void Snake::updateTexCoords()
{
	for (size_t i = 0; i < segments.size(); ++i) {
        const float normalizedPosition = static_cast<float>(i) / (segments.size() - 1);
        sf::Vertex* triangles = &vertices[i * TRIANGLE_VERTICES];
        triangles[0].texCoords = { 0, normalizedPosition };
        triangles[1].texCoords = { 1, normalizedPosition };
        triangles[2].texCoords = triangles[0].texCoords;
        triangles[3].texCoords = triangles[1].texCoords;
        triangles[4].texCoords = triangles[0].texCoords;
        triangles[5].texCoords = triangles[1].texCoords;
	}
}

void Snake::updateVertices(const float dt)
{
    for (size_t i = 0; i < segments.size(); ++i) {
        const float posX = (prevSegments[i].x + (segments[i].x - prevSegments[i].x) * dt) * config.size;
        const float posY = (prevSegments[i].y + (segments[i].y - prevSegments[i].y) * dt) * config.size;
		const float posXEnd = posX + config.size;
		const float posYEnd = posY + config.size;

        sf::Vertex* triangles = &vertices[i * TRIANGLE_VERTICES];
        triangles[0].position = { posX, posY };
        triangles[1].position = { posXEnd, posY };
        triangles[2].position = { posXEnd, posYEnd };
        triangles[3].position = triangles[2].position;
        triangles[4].position = { posX, posYEnd };
        triangles[5].position = triangles[0].position;
    }
}

void Snake::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(vertices, states);
}
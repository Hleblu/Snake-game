#include "snake.h"
#include "collisionManager.h"
#include "configuration.h"
#include <SFML/Graphics/RenderTarget.hpp>

Snake::Snake()
{
    vertices.setPrimitiveType(sf::PrimitiveType::Triangles);
    restoreDefaultValues();
}

void Snake::restoreDefaultValues()
{
    collisionManager.clearMap();

    const std::int16_t centerX = config.rows / 2;
    const std::int16_t centerY = config.columns / 2;

    segments = {
        {centerX, centerY},
        {static_cast<std::int16_t>(centerX - 1), centerY},
        {static_cast<std::int16_t>(centerX - 2), centerY}
    };
    previousSegments = segments;

    collisionManager.setOccupied(segments[0], SNAKE_HEAD);
    collisionManager.setOccupied(segments[1], SNAKE_TAIL);
    collisionManager.setOccupied(segments[2], SNAKE_TAIL);

    direction = NONE, previousDirection = NONE, nextDirection = NONE;
    firstMove = true;
    hashDelay = 0;

    vertices.clear();
    vertices.resize(config.rows * config.columns * 6);

    updateVertices();
	updateTexCoords();
}

bool Snake::hasCollided() const
{
    return collisionManager.checkCellType(segments[0], SNAKE_TAIL)
        || collisionManager.checkCellType(segments[0], OBSTACLE)
        || segments[0].x < 0 || segments[0].x > config.rows - 1
        || segments[0].y < 0 || segments[0].y > config.columns - 1;
}

void Snake::grow(const int size)
{
    hashDelay += size;
    for (int i = 0; i < size; ++i) {
        segments.emplace_back(segments.back());
        previousSegments.emplace_back(previousSegments.back());
    }
	updateTexCoords();
}

void Snake::move()
{
    if (direction == NONE) return;
    
    if (!firstMove) { 
        previousSegments.pop_back();
        previousSegments.emplace_front(segments.front());
    }
    else firstMove = false;

    if (hashDelay == 0)
        collisionManager.setFree(segments.back(), SNAKE_TAIL);
    else hashDelay -= 1;
    
    segments.pop_back();
    segments.emplace_front(segments.front());
    collisionManager.changeTypes(segments.front(), SNAKE_HEAD, SNAKE_TAIL);

    switch (direction) {
        case RIGHT: segments[0].x++; break;
        case LEFT: segments[0].x--; break;
        case UP: segments[0].y--; break;
        case DOWN: segments[0].y++; break;
        default: break;
    }

    collisionManager.setOccupied(segments.front(), ObjectType::SNAKE_HEAD);
    previousDirection = direction;
}

bool Snake::canUpdateDirection() const
{
    return nextDirection != direction && nextDirection % 2 != previousDirection % 2;
}

const std::deque<Cell>& Snake::getSegments() const 
{
    return segments;
}

void Snake::updateTexCoords()
{
	for (size_t i = 0; i < segments.size(); ++i) {
        const float normalizedPosition = static_cast<float>(i) / (segments.size() - 1);
        sf::Vertex* triangles = &vertices[i * 6];
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
        const float posX = (previousSegments[i].x + (segments[i].x - previousSegments[i].x) * dt) * config.size;
        const float posY = (previousSegments[i].y + (segments[i].y - previousSegments[i].y) * dt) * config.size;
		const float posXEnd = posX + config.size;
		const float posYEnd = posY + config.size;

        sf::Vertex* triangles = &vertices[i * 6];
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
#include "snake.h"

Snake::Snake()
{
    segmentsVertices.setPrimitiveType(sf::PrimitiveType::Triangles);
    segmentsSet.reserve(config->rows * config->columns);
    restoreDefaultValues();
}

void Snake::restoreDefaultValues()
{
    short int centerX = config->rows / 2;
    short int centerY = config->columns / 2;

    segments = {
        {centerX, centerY},
        {static_cast<short>(centerX - 1), centerY},
        {static_cast<short>(centerX - 2), centerY}
    };
    previousSegments = segments;
    segmentsSet = { segments[0], segments[1], segments[2] };

    direction = NONE, previousDirection = NONE, nextDirection = NONE;
    firstMove = true;

    segmentsVertices.clear();
    segmentsVertices.resize(config->rows * config->columns * 6);

    updateVertices();
	updateTexCoords();
}

bool Snake::hasCollided()
{
    return segmentsSet.count(segments[0]) > 1 
        || unsigned(segments[0].x) > config->rows - 1 
        || unsigned(segments[0].y) > config->columns - 1;
}

void Snake::grow()
{
    segments.emplace_back(segments.back());
    previousSegments.emplace_back(previousSegments.back());
	updateTexCoords();
}

void Snake::move()
{
    if (direction == NONE) return;
    
    if (!firstMove) { previousSegments.pop_back(); previousSegments.emplace_front(segments.front()); }
    firstMove = false;

    segmentsSet.erase(segments.back());
    segments.pop_back();
    segments.emplace_front(segments.front());

    switch (direction) {
    case RIGHT: segments[0].x++; break;
    case LEFT: segments[0].x--; break;
    case UP: segments[0].y--; break;
    case DOWN: segments[0].y++; break;
    default: break;
    }

    segmentsSet.insert(segments.front());
    previousDirection = direction;
}

bool Snake::canUpdateDirection()
{
    return nextDirection != direction && nextDirection % 2 != previousDirection % 2;
}

void Snake::updateTexCoords()
{
	for (float i = 0; i < segments.size(); ++i) {
        const float normalizedPosition = i / (segments.size() - 1);
        sf::Vertex* triangles = &segmentsVertices[i * 6];
        triangles[0].texCoords = { 0, normalizedPosition };
        triangles[1].texCoords = { 1, normalizedPosition };
        triangles[2].texCoords = triangles[0].texCoords;
        triangles[3].texCoords = triangles[1].texCoords;
        triangles[4].texCoords = triangles[0].texCoords;
        triangles[5].texCoords = triangles[1].texCoords;
	}
}

void Snake::updateVertices(float dt)
{
    for (size_t i = 0; i < segments.size(); ++i) {
        const float posX = (previousSegments[i].x + (segments[i].x - previousSegments[i].x) * dt) * config->size;
        const float posY = (previousSegments[i].y + (segments[i].y - previousSegments[i].y) * dt) * config->size;

        sf::Vertex* triangles = &segmentsVertices[i * 6];
        triangles[0].position = { posX, posY };
        triangles[1].position = { posX + config->size, posY };
        triangles[2].position = { posX + config->size, posY + config->size };
        triangles[3].position = triangles[2].position;
        triangles[4].position = { posX, posY + config->size };
        triangles[5].position = triangles[0].position;
    }
}

void Snake::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(segmentsVertices, states);
}
#include "snake.h"

Snake::Snake()
{
    segmentsVertices.setPrimitiveType(sf::PrimitiveType::Triangles);
    segmentsSet.reserve(gridX * gridY);
    restoreDefaultValues();
}

void Snake::restoreDefaultValues()
{
    segments = {
        {gridX / 2, gridY / 2},
        {gridX / 2 - 1, gridY / 2},
        {gridX / 2 - 2, gridY / 2}
    };
    previousSegments = segments;
    segmentsSet = { segments[0], segments[1], segments[2] };

    direction = NONE, previousDirection = NONE, nextDirection = NONE;
    firstMove = true;

    segmentsVertices.clear();
    segmentsVertices.resize(gridX * gridY);

    updateColors();
    updateVertexArray();
}

bool Snake::hasCollided()
{
    return segmentsSet.count(segments[0]) > 1 
        || unsigned(segments[0].x) > gridX - 1 
        || unsigned(segments[0].y) > gridY - 1;
}

void Snake::grow()
{
    segments.emplace_back(segments.back());
    previousSegments.emplace_back(previousSegments.back());
    updateColors();
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

void Snake::updateColors() {
    colorDecrementStep = segmentColor.b / segments.size();
    sf::Color color = segmentColor;
    for (int i = 0; i < segments.size(); ++i) {
        sf::Vertex* triangles = &segmentsVertices[i * 6];
        for (int j = 0; j < 6; ++j) {
            triangles[j].color = color;
        }
        color.b -= colorDecrementStep;
    }
}

void Snake::updateVertexArray(float dt)
{
    for (int i = 0; i < segments.size(); ++i) {
        float posX = (previousSegments[i].x + (segments[i].x - previousSegments[i].x) * dt) * size;
        float posY = (previousSegments[i].y + (segments[i].y - previousSegments[i].y) * dt) * size;

        sf::Vertex* triangles = &segmentsVertices[i * 6];
        triangles[0].position = sf::Vector2f(posX, posY);
        triangles[1].position = sf::Vector2f(posX + size, posY);
        triangles[2].position = sf::Vector2f(posX + size, posY + size);
        triangles[3].position = sf::Vector2f(posX + size, posY + size);
        triangles[4].position = sf::Vector2f(posX, posY + size);
        triangles[5].position = sf::Vector2f(posX, posY);
    }
}

void Snake::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(segmentsVertices, states);
}